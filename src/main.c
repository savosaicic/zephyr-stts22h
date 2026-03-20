#include <zephyr/kernel.h>
#include <zephyr/device.h>
#include <zephyr/logging/log.h>
#include <zephyr/drivers/sensor.h>

LOG_MODULE_REGISTER(zephyr_stts22h, LOG_LEVEL_DBG);

static const struct device *const dev = DEVICE_DT_GET(DT_NODELABEL(stts22h));

int main(void)
{
  int ret;
  struct sensor_value temperature;

  if (!device_is_ready(dev)) {
    LOG_ERR("stts22h not ready");
    return -ENODEV;
  }

  while (1) {
    ret = sensor_sample_fetch(dev);
    if (ret < 0) {
      LOG_ERR("sensor_sample_fetch() failed: %d", ret);
      k_msleep(1000);
      continue;
    }

    ret = sensor_channel_get(dev, SENSOR_CHAN_AMBIENT_TEMP, &temperature);
    if (ret < 0) {
      LOG_ERR("sensor_channel_get() failed: %d", ret);
      k_msleep(1000);
      continue;
    }

    LOG_INF("temperature: %.2f °C", sensor_value_to_double(&temperature));

    k_msleep(1000);
  }
}
