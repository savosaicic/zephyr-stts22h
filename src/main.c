#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

LOG_MODULE_REGISTER(zephyr_stts22h, LOG_LEVEL_DBG);

static const struct device *const dev = DEVICE_DT_GET(DT_NODELABEL(stts22h));

static void threshold_handler(const struct device *dev,
                              const struct sensor_trigger *trig)
{
  int ret;
  struct sensor_value temperature;

  ret = sensor_sample_fetch(dev);
  if (ret < 0) {
    LOG_ERR("sensor_sample_fetch() failed: %d", ret);
    return;
  }

  ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temperature);
  if (ret < 0) {
    LOG_ERR("sensor_channel_get() failed: %d", ret);
    return;
  }

  LOG_WRN("Threshold crossed! temperature: %.2f °C",
          sensor_value_to_double(&temperature));
}

int main(void)
{
  int ret;

  if (!device_is_ready(dev)) {
    LOG_ERR("stts22h not ready");
    return -ENODEV;
  }

  struct sensor_value upper_thres = {.val1 = 28, .val2 = 0};
  ret = sensor_attr_set(dev, SENSOR_CHAN_AMBIENT_TEMP, SENSOR_ATTR_UPPER_THRESH,
                        &upper_thres);
  if (ret < 0) {
    LOG_ERR("Failed to set upper threshold: %d", ret);
    return ret;
  }

  struct sensor_value lower_thres = {.val1 = 18, .val2 = 0};
  ret = sensor_attr_set(dev, SENSOR_CHAN_AMBIENT_TEMP, SENSOR_ATTR_LOWER_THRESH,
                        &lower_thres);
  if (ret < 0) {
    LOG_ERR("Failed to set lower threshold: %d", ret);
    return ret;
  }

  struct sensor_trigger trig = {
    .type = SENSOR_TRIG_THRESHOLD,
    .chan = SENSOR_CHAN_AMBIENT_TEMP,
  };
  ret = sensor_trigger_set(dev, &trig, threshold_handler);
  if (ret < 0) {
    LOG_ERR("Failed to set trigger: %d", ret);
    return ret;
  }

  k_sleep(K_FOREVER);
}
