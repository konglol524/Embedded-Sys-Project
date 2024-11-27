#include <Arduino.h>
#include <driver/i2s.h>

#define MIC_I2S_WS 15
#define MIC_I2S_SCK 2
#define MIC_I2S_SD 13
// uncounnected L/R pins mean L
#define I2S_PORT I2S_NUM_0
// put function declarations here:

void i2s_install()
{
  const i2s_config_t i2s_config = {
      .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = 44100,
      .bits_per_sample = i2s_bits_per_sample_t(16),
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
      .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
      .intr_alloc_flags = 0,
      .dma_buf_count = 8,
      .dma_buf_len = 64,
      .use_apll = false};
  i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  Serial.println("I2S installed.");
};

void i2s_setpin()
{
  const i2s_pin_config_t pin_config = {
      .bck_io_num = MIC_I2S_SCK,
      .ws_io_num = MIC_I2S_WS,
      .data_out_num = -1,
      .data_in_num = MIC_I2S_SD};

  i2s_set_pin(I2S_PORT, &pin_config);
}

esp_err_t err;
esp_err_t err_pin;
esp_err_t err_start;
void setup()
{
  Serial.begin(460800);
  delay(16080);
  Serial.println("Set up IS2 ...");
  // i2s_install();
  // i2s_setpin();
  const i2s_config_t i2s_config = {
      .mode = i2s_mode_t(I2S_MODE_MASTER | I2S_MODE_RX),
      .sample_rate = 44100,
      .bits_per_sample = i2s_bits_per_sample_t(16),
      .channel_format = I2S_CHANNEL_FMT_ONLY_LEFT,
      .communication_format = i2s_comm_format_t(I2S_COMM_FORMAT_STAND_I2S),
      .intr_alloc_flags = 0,
      .dma_buf_count = 8,
      .dma_buf_len = 64,
      .use_apll = false};
  err = i2s_driver_install(I2S_PORT, &i2s_config, 0, NULL);
  if (err != ESP_OK)
  {
    Serial.print("Error installing I2S driver: ");
    Serial.println(esp_err_to_name(err)); // Converts the error code to a human-readable string
  }
  else
  {
    Serial.println("I2S driver installed successfully.");
  };
  delay(1000);
  const i2s_pin_config_t pin_config = {
      .bck_io_num = MIC_I2S_SCK,
      .ws_io_num = MIC_I2S_WS,
      .data_out_num = -1,
      .data_in_num = MIC_I2S_SD};

  err_pin = i2s_set_pin(I2S_PORT, &pin_config);
  if (err_pin != ESP_OK)
  {
    Serial.print("Error setting up pin: ");
    Serial.println(esp_err_to_name(err_pin)); // Converts the error code to a human-readable string
  }
  else
  {
    Serial.println("Set Pin sucessfully");
  };
  delay(1000);

  err_start = i2s_start(I2S_PORT);
    if (err_start != ESP_OK)
  {
    Serial.print("Error starting: ");
    Serial.println(esp_err_to_name(err_start)); // Converts the error code to a human-readable string
  }
  else
  {
    Serial.println("Started sucessfully");
  };
  delay(1000);
}

void loop()
{
  if (err != ESP_OK) {
    Serial.print("I2S install failed with error: ");
    Serial.println(esp_err_to_name(err));
  } 
  if (err_pin != ESP_OK) {
    Serial.print("I2S install failed with error: ");
    Serial.println(esp_err_to_name(err_pin));
  } 

  if (err_start != ESP_OK)
  {
    Serial.print("Error starting: ");
    Serial.println(esp_err_to_name(err_start));
  }
  // Serial.println("Looping ...");
  //  Buffer to store audio samples
  int32_t sample_buffer[64];
  size_t bytes_read;
  // put your main code here, to run repeatedly:
  int bytes = i2s_read(I2S_PORT, sample_buffer, sizeof(sample_buffer), &bytes_read, portMAX_DELAY); //no delay
  // Serial.println(bytes);
  if (bytes == ESP_OK)
  {
    // Calculate number of samples read
    int num_samples = bytes_read / sizeof(int32_t);

    // Print samples to the Serial Monitor
    for (int i = 0; i < num_samples; i++)
    {
      Serial.println(sample_buffer[i]);
    }
  }
}