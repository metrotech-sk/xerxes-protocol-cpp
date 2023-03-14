#ifndef __ESPUART_HPP
#define __ESPUART_HPP

#include "Network.hpp"
#include "Message.hpp"
#include "esp_err.h"
#include "driver/uart.h"
#include <vector>

namespace Xerxes
{


class EspUart : public Network
{
private:
    const uart_port_t uart_num = UART_NUM_2;
    bool receivePacket(Packet &packet);
    void txEnable() const;
    void txDisable() const;
    const char* TAG = "EspUart";

    gpio_num_t gpio_num_tx_en;
    
public:
    EspUart(const int uart_pin_tx, const int uart_pin_rx, const int uart_pin_tx_en);
    ~EspUart();

    bool sendData(const Packet & toSend) const override;
    bool readData(const uint64_t timeoutUs, Packet &packet) override;
};



EspUart::EspUart(const int uart_pin_tx, const int uart_pin_rx, const int uart_pin_tx_en)
{
    ESP_LOGD(TAG, "Initializing UART config");
    uart_config_t uart_config = {
        .baud_rate = 115200,
        .data_bits = UART_DATA_8_BITS,
        .parity = UART_PARITY_DISABLE,
        .stop_bits = UART_STOP_BITS_1,
        .flow_ctrl = UART_HW_FLOWCTRL_DISABLE,
        .rx_flow_ctrl_thresh = 122,
    };
    // Configure UART parameters
    ESP_LOGD(TAG, "Configuring UART");
    ESP_ERROR_CHECK(uart_param_config(uart_num, &uart_config));
    
    // Set UART pins
    ESP_LOGD(TAG, "Setting UART pins");
    ESP_ERROR_CHECK(uart_set_pin(uart_num, uart_pin_tx, uart_pin_rx, uart_pin_tx_en, UART_PIN_NO_CHANGE));
    
    // Install UART driver, and get the queue.
    ESP_LOGD(TAG, "Installing UART driver");
    ESP_ERROR_CHECK(uart_driver_install(uart_num, 0x100, 0x100, 0, NULL, 0));

    // Set UART mode to RS485 Half Duplex - auto tx enable
    ESP_ERROR_CHECK(uart_set_mode(uart_num, UART_MODE_RS485_HALF_DUPLEX));
}

EspUart::~EspUart()
{
}

bool EspUart::sendData(const Packet & toSend) const
{
    ESP_LOGD(TAG, "Preparing data");
    const size_t size = toSend.size();
    uint8_t data[size] = {0};
    for (size_t i = 0; i < size; i++)
    {
        data[i] = toSend.at(i);
        ESP_LOGV(TAG, "data[%d] = %d", i, data[i]);
    }

    ESP_LOGD(TAG, "Sending data");
    const int txBytes = uart_write_bytes(uart_num, (const char *)data, size);

    return txBytes == size;
}

bool EspUart::readData(const uint64_t timeoutUs, Packet &packet)
{
    ESP_LOGD(TAG, "Reading data from UART");
    auto startTimeUs = esp_timer_get_time();

    // while we have time to read
    while(esp_timer_get_time() - startTimeUs < timeoutUs)
    {
        // wait for SOH
        uint8_t sohByte;
        auto remainingTimeUs = timeoutUs - (esp_timer_get_time() - startTimeUs);
        auto remainingRtosTicks = pdMS_TO_TICKS(remainingTimeUs / 1000);

        ESP_LOGD(TAG, "Reading SOH");
        int rxBytes = uart_read_bytes(uart_num, &sohByte, 1, remainingRtosTicks);
        if (rxBytes == 0)
        {
            // failed to read SOH in timeout
            ESP_LOGD(TAG, "Failed to read SOH in timeout");
            return false;
        }

        // if we received SOH
        if (sohByte != SOH)
        {
            // keep reading until we get SOH
            ESP_LOGD(TAG, "Received byte is not SOH");
            continue;
        }


        //read length of packet
        uint8_t lengthByte;
        remainingTimeUs = timeoutUs - (esp_timer_get_time() - startTimeUs);
        remainingRtosTicks = pdMS_TO_TICKS(remainingTimeUs / 1000);

        ESP_LOGD(TAG, "Reading length");
        rxBytes = uart_read_bytes(uart_num, &lengthByte, 1, remainingRtosTicks);

        if (rxBytes == 0)
        {
            // failed to read length in timeout
            ESP_LOGD(TAG, "Failed to read length in timeout");
            return false;
        }
        
        if(lengthByte < 1 || lengthByte > 255)
        {
            // invalid length, keep reading until we get SOH
            ESP_LOGD(TAG, "Invalid length: %d", lengthByte);
            continue;
        }

        ESP_LOGD(TAG, "Received Length: %d", lengthByte);

        // read the data from packet (lengthByte - 3) because we already read SOH, lengthByte and checksum
        uint8_t data[lengthByte - 3] = {0};
        remainingTimeUs = timeoutUs - (esp_timer_get_time() - startTimeUs);
        remainingRtosTicks = pdMS_TO_TICKS(remainingTimeUs / 1000);
        
        ESP_LOGD(TAG, "Reading payload");
        rxBytes = uart_read_bytes(uart_num, data, lengthByte - 3, remainingRtosTicks);
        if (rxBytes != lengthByte - 3)
        {
            // failed to read data in timeout
            ESP_LOGD(TAG, "Failed to read data in timeout");
            return false;
        }

        // read checksum
        uint8_t checksumByte;
        remainingTimeUs = timeoutUs - (esp_timer_get_time() - startTimeUs);
        remainingRtosTicks = pdMS_TO_TICKS(remainingTimeUs / 1000);

        rxBytes = uart_read_bytes(uart_num, &checksumByte, 1, remainingRtosTicks);
        if (rxBytes == 0)
        {
            // failed to read checksum in timeout
            ESP_LOGD(TAG, "Failed to read checksum in timeout");
            return false;
        }

        
        // calculate checksum
        uint8_t calculatedChecksum = SOH;
        calculatedChecksum += lengthByte;
        for (size_t i = 0; i < lengthByte - 3; i++)
        {
            calculatedChecksum += data[i];
        }
        calculatedChecksum += checksumByte;
        ESP_LOGD(TAG, "Calculated checksum: %d = OK", calculatedChecksum);

        if(calculatedChecksum != 0)
        {
            // invalid checksum, keep reading until we get SOH
            ESP_LOGD(TAG, "Invalid checksum: %d", calculatedChecksum);
            continue;
        }
        else
        {
            // valid packet
            std::vector<uint8_t> packetData;
            packetData.reserve(lengthByte);

            for (size_t i = 0; i < lengthByte - 3; i++)
            {
                packetData.push_back(data[i]);
            }

            packet = Packet(packetData);

            // return true if we received a valid packet
            return true;
        }
    }

    // return false if we didn't receive a valid packet in timeout
    return false;
}


} // namespace Xerxes


#endif // !__ESPUART_HPP