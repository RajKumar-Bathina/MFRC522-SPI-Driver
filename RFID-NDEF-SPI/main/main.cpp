/*
* Author : Raj Kumar Bathina
* Date   : Feb 2026
* License: MIT 
*/

#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include <esp_log.h>
#include <esp_check.h>
#include <string.h>
#include <time.h>
#include "esp_log.h"
#include <stdlib.h>
#include "MFRC522_SPI.h"
#include "driver/spi_master.h"
#include "driver/gpio.h"
#include "NfcAdapter.h"

static const char *TAG = "MFRC522_SPI";

spi_device_handle_t spiHandle;

void spi_master_init()
{
    esp_err_t ret;

    // Zero-initialize the structs first!
    spi_bus_config_t buscfg = {};
    buscfg.miso_io_num = PIN_NUM_MISO;
    buscfg.mosi_io_num = PIN_NUM_MOSI;
    buscfg.sclk_io_num = PIN_NUM_CLK;
    buscfg.quadwp_io_num = -1;
    buscfg.quadhd_io_num = -1;
    // Optionally set: buscfg.max_transfer_sz = 0;

    spi_device_interface_config_t devcfg = {};
    devcfg.clock_speed_hz = 5000000; // 5 MHz
    devcfg.mode = 0;
    devcfg.spics_io_num = PIN_NUM_CS;
    devcfg.queue_size = 7;
    // Optionally set: devcfg.flags = 0;

    // Initialize the SPI bus
    ret = spi_bus_initialize(SPI2_HOST, &buscfg, SPI_DMA_CH_AUTO);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "SPI bus initialize failed: %s", esp_err_to_name(ret));
        return;
    }
    assert(ret == ESP_OK);

    // Attach the RFID to the SPI bus
    ret = spi_bus_add_device(SPI2_HOST, &devcfg, &spiHandle);
    if (ret != ESP_OK)
    {
        ESP_LOGE(TAG, "SPI device add failed: %s", esp_err_to_name(ret));
        return;
    }
    assert(ret == ESP_OK);

    ESP_LOGI(TAG, "SPI initialized successfully");
}

extern "C" void app_main()
{
    ESP_LOGI(TAG, "Initializing SPI driver...");
    spi_master_init();
    vTaskDelay(100 / portTICK_PERIOD_MS); // Give SPI time to settle

    static MFRC522 mfrc522(spiHandle);
    NfcAdapter nfc(&mfrc522);

    ESP_LOGI(TAG, "Initializing MFRC522...");
    mfrc522.PCD_Init(); // Init MFRC522

    ESP_LOGI(TAG, "Initializing NFC adapter...");
    nfc.begin();

    while (1)
    {
        ESP_LOGI(TAG, "Checking for NFC tag...");
        if (nfc.tagPresent())
        {
            ESP_LOGI(TAG, "Reading NFC tag");
            NfcTag tag = nfc.read();
            tag.print();
            vTaskDelay(pdMS_TO_TICKS(5000));
        }
        // else {
        //     ESP_LOGI(TAG, "No NFC tag present");
        // }
        vTaskDelay(pdMS_TO_TICKS(1000)); // Check every second
    }
}
