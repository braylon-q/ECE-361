#include <stdio.h>
#include <stdint.h>
#include <time.h>
#include "../iom361_r4.h"
#include "../scenario.h"
#include "../panel.h"
#include "../sensor.h"

static int fails = 0;
static int passes = 0;

#define CHECK(cond) do { \
    if (cond) { \
        printf("PASS: CHECK(%s)\n", #cond); \
        passes++; \
    } \
    else { \
        printf("FAIL: CHECK(%s)\n", #cond); \
        fails++; \
    } \
} while (0)

/*
 * Load scenario text from a temporary stream so scenario tests do not need
 * external files or modify the repository.
 */
static int load_scenario_text(const char *text, scenario_t *scene,
                              time_t *start_time)
{
    FILE *input = tmpfile();
    if (input == NULL) {
        return -1;
    }

    if (fputs(text, input) == EOF) {
        fclose(input);
        return -1;
    }

    rewind(input);
    int result = scenario_load(input, scene, start_time);
    fclose(input);
    return result;
}

/*
 * Return the most recently specified scenario values at a requested tick.
 * Scenario rows are change points, so values from the preceding row hold
 * until the next row's tick.
 */
static void scenario_values_at(const scenario_t *scene, uint32_t tick,
                               float *temp, float *rh, uint32_t *switches)
{
    int entry = 0;
    for (int i = 1; i < scene->size && scene->tick[i] <= tick; i++) {
        entry = i;
    }

    *temp = scene->temp[entry];
    *rh = scene->rh[entry];
    *switches = scene->switches[entry];
}

int main(void)
{
    int rc = 0;
    uint32_t *base = iom361_initialize(4, 4, &rc);

    /* Confirm the emulator is available before using it for sensor checks. */
    printf("Initialize the I/O emulator for sensor tests.\n");
    CHECK(base != NULL && rc == 0);

    if (base != NULL && rc == 0) {
        float temp = -1.0f;
        float rh = -1.0f;
        iom361_set_display(false);

        /* Verify that a high in-range temperature survives sensor conversion. */
        printf("A sensor reading set to 91.0 C reads back as 91.0 C.\n");
        _iom361_set_sensor1(91.0f, 50.0f);
        CHECK(sensor_read(base, &temp, &rh) == 0 && temp == 91.0f);

        /* A rejected read must not overwrite either caller-provided value. */
        printf("A failed sensor read leaves both output values unchanged.\n");
        temp = 12.5f;
        rh = 67.5f;
        CHECK(sensor_read(NULL, &temp, &rh) == -1 &&
              temp == 12.5f && rh == 67.5f);
    }

    /* Check the rising edge, falling edge, and unchanged switch states. */
    printf("A newly-on switch is reported as pressed.\n");
    CHECK(panel_pressed(0x1U, 0x0U) == 0x1U);

    printf("A newly-off switch is reported as released.\n");
    CHECK(panel_released(0x0U, 0x1U) == 0x1U);

    printf("A held switch is neither newly pressed nor newly released.\n");
    CHECK(panel_pressed(0x1U, 0x1U) == 0U &&
          panel_released(0x1U, 0x1U) == 0U);

    /* Start with multiple LEDs set so each operation must preserve neighbors. */
    printf("Turning one LED on preserves every other LED.\n");
    uint32_t leds = 0xAU;
    CHECK(panel_led(&leds, 2, 1) == 0 && leds == 0xEU);

    printf("Turning one LED off preserves every other LED.\n");
    CHECK(panel_led(&leds, 1, 0) == 0 && leds == 0xCU);

    printf("Toggling one LED preserves every other LED.\n");
    leds = 0xAU;
    CHECK(panel_toggle(&leds, 2) == 0 && leds == 0xEU);

    /* Include comments, whitespace-only lines, and trailing inline comments. */
    printf("A scenario loader ignores comments and blank lines.\n");
    scenario_t scene;
    time_t start_time = 0;
    const char *valid_scenario =
        "# comment before start\n"
        "\n"
        "  start 1700000000 # start timestamp\n"
        "\n"
        "# tick temp rh switches\n"
        "0 12.0 40.0 0x0\n"
        "5 20.0 50.0 0x1 # switch changes\n";
    rc = load_scenario_text(valid_scenario, &scene, &start_time);
    CHECK(rc == 0 && start_time == (time_t)1700000000 &&
          scene.size == 2 && scene.tick[1] == 5);

    /* Verify an intermediate tick still uses the most recent scenario row. */
    printf("Scenario values hold between successive entry ticks.\n");
    float held_temp = 0.0f;
    float held_rh = 0.0f;
    uint32_t held_switches = 0;
    scenario_values_at(&scene, 3, &held_temp, &held_rh, &held_switches);
    CHECK(held_temp == 12.0f && held_rh == 40.0f && held_switches == 0x0U);

    /* Each malformed scenario below must be rejected by the loader. */
    printf("A scenario without a start line is rejected.\n");
    rc = load_scenario_text("# no start line\n0 12.0 40.0 0x0\n",
                            &scene, &start_time);
    CHECK(rc != 0);

    printf("A scenario entry with a missing field is rejected.\n");
    rc = load_scenario_text("start 1700000000\n2 12.0 40.0 0x0\n"
                            "3 13.0 41.0\n",
                            &scene, &start_time);
    CHECK(rc != 0);

    printf("Scenario ticks that do not increase are rejected.\n");
    rc = load_scenario_text("start 1700000000\n5 12.0 40.0 0x0\n"
                            "4 13.0 41.0 0x1\n",
                            &scene, &start_time);
    CHECK(rc != 0);

    printf("\n--SUMMARY--\n");
    printf("%d/%d tests passed.\n\n", passes, passes + fails);
    return fails != 0;
}
