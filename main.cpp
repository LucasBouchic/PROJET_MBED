#include "mbed.h"
#include "HTU21DF.h"
#include "AS6212.h"
#include "PressureSensor.h"

#include <stdio.h>

#include "lorawan/LoRaWANInterface.h"
#include "lorawan/system/lorawan_data_structures.h"
#include "events/EventQueue.h"
#include "lora_radio_helper.h"

I2C i2c(P1_I2C_SDA, P1_I2C_SCL);

// Application helpers

using namespace events;

// Max payload size can be LORAMAC_PHY_MAXPAYLOAD.
// This example only communicates with much shorter messages (<30 bytes).
// If longer messages are used, these buffers must be changed accordingly.
uint8_t tx_buffer[100];
uint8_t rx_buffer[30];

/*
 * Sets up an application dependent transmission timer in ms. Used only when Duty Cycling is off for testing
 */
#define TX_TIMER 10000

/**
 * Maximum number of events for the event queue.
 * 10 is the safe number for the stack events, however, if application
 * also uses the queue for whatever purposes, this number should be increased.
 */
#define MAX_NUMBER_OF_EVENTS 10

/**
 * Maximum number of retries for CONFIRMED messages before giving up
 */
#define CONFIRMED_MSG_RETRY_COUNTER 3

/**
 * Dummy sensor class object
 */
char *payload = "{\"temperature\": 32.5}";


/**
 * This event queue is the global event queue for both the
 * application and stack. To conserve memory, the stack is designed to run
 * in the same thread as the application and the application is responsible for
 * providing an event queue to the stack that will be used for ISR deferment as
 * well as application information event queuing.
 */
static EventQueue ev_queue(MAX_NUMBER_OF_EVENTS *EVENTS_EVENT_SIZE);

/**
 * Event handler.
 *
 * This will be passed to the LoRaWAN stack to queue events for the
 * application which in turn drive the application.
 */
static void lora_event_handler(lorawan_event_t event);

/**
 * Constructing Mbed LoRaWANInterface and passing it the radio object from lora_radio_helper.
 */
static LoRaWANInterface lorawan(radio);

/**
 * Application specific callbacks
 */
static lorawan_app_callbacks_t callbacks;

HTU21DF htu21df(i2c);
AS6212 as6212(i2c);
PressureSensor pressureSensor(i2c);

/**
 * Sends a message to the Network Server
 */
static void send_message()
{
    uint16_t packet_len;
    int16_t retcode;
    int32_t sensor_value;

        float temp = htu21df.readTemperature();

        float humidity = htu21df.readHumidity();

        float as6212Temp = as6212.readTemperature();

    snprintf((char *)tx_buffer, sizeof(tx_buffer), "{\"temperature\":%.2f,\"humidity\":%.2f,\"as6212Temp\":%.2f}",
             temp, humidity, as6212Temp);

    packet_len = strlen((char *)tx_buffer);

    retcode = lorawan.send(MBED_CONF_LORA_APP_PORT, tx_buffer, packet_len, MSG_UNCONFIRMED_FLAG);

    if (retcode < 0)
    {
        printf("Error %d while sending data\r\n", retcode);
        return;
    }

    printf("Payload sent: %s\r\n", tx_buffer);
}

/**
 * Receive a message from the Network Server
 */
static void receive_message()
{
    uint8_t port;
    int flags;
    int16_t retcode = lorawan.receive(rx_buffer, sizeof(rx_buffer), port, flags);

    if (retcode < 0)
    {
        printf("\r\n receive() - Error code %d \r\n", retcode);
        return;
    }

    printf(" RX Data on port %u (%d bytes): ", port, retcode);
    for (uint8_t i = 0; i < retcode; i++)
    {
        printf("%02x ", rx_buffer[i]);
    }
    printf("\r\n");

    memset(rx_buffer, 0, sizeof(rx_buffer));
}

/**
 * Event handler
 */
static void lora_event_handler(lorawan_event_t event)
{
    switch (event)
    {
    case CONNECTED:
        printf("\r\n Connection - Successful \r\n");
        if (MBED_CONF_LORA_DUTY_CYCLE_ON)
        {
            send_message();
        }
        else
        {
            ev_queue.call_every(TX_TIMER, send_message);
        }

        break;
    case DISCONNECTED:
        ev_queue.break_dispatch();
        printf("\r\n Disconnected Successfully \r\n");
        break;
    case TX_DONE:
        printf("\r\n Message Sent to Network Server \r\n");
        if (MBED_CONF_LORA_DUTY_CYCLE_ON)
        {
            send_message();
        }
        break;
    case TX_TIMEOUT:
    case TX_ERROR:
    case TX_CRYPTO_ERROR:
    case TX_SCHEDULING_ERROR:
        printf("\r\n Transmission Error - EventCode = %d \r\n", event);
        // try again
        if (MBED_CONF_LORA_DUTY_CYCLE_ON)
        {
            send_message();
        }
        break;
    case RX_DONE:
        printf("\r\n Received message from Network Server \r\n");
        receive_message();
        break;
    case RX_TIMEOUT:
    case RX_ERROR:
        printf("\r\n Error in reception - Code = %d \r\n", event);
        break;
    case JOIN_FAILURE:
        printf("\r\n OTAA Failed - Check Keys \r\n");
        break;
    case UPLINK_REQUIRED:
        printf("\r\n Uplink required by NS \r\n");
        if (MBED_CONF_LORA_DUTY_CYCLE_ON)
        {
            send_message();
        }
        break;
    default:
        MBED_ASSERT("Unknown Event");
    }
}


int main(void)
{

    // stores the status of a call to LoRaWAN protocol
    lorawan_status_t retcode;


        // Initialize LoRaWAN stack
        if (lorawan.initialize(&ev_queue) != LORAWAN_STATUS_OK)
        {
            printf("\r\n LoRa initialization failed! \r\n");
            return -1;
        }

        printf("\r\n Mbed LoRaWANStack initialized \r\n");

        // prepare application callbacks
        callbacks.events = mbed::callback(lora_event_handler);
        lorawan.add_app_callbacks(&callbacks);

        // Set number of retries in case of CONFIRMED messages
        if (lorawan.set_confirmed_msg_retries(CONFIRMED_MSG_RETRY_COUNTER) != LORAWAN_STATUS_OK)
        {
            printf("\r\n set_confirmed_msg_retries failed! \r\n\r\n");
            return -1;
        }

        printf("\r\n CONFIRMED message retries : %d \r\n",
               CONFIRMED_MSG_RETRY_COUNTER);

        // Enable adaptive data rate
        if (lorawan.enable_adaptive_datarate() != LORAWAN_STATUS_OK)
        {
            printf("\r\n enable_adaptive_datarate failed! \r\n");
            return -1;
        }

        printf("\r\n Adaptive data  rate (ADR) - Enabled \r\n");

        retcode = lorawan.connect();

        if (retcode == LORAWAN_STATUS_OK ||
            retcode == LORAWAN_STATUS_CONNECT_IN_PROGRESS)
        {
        }
        else
        {
            printf("\r\n Connection error, code = %d \r\n", retcode);
            return -1;
        }
        

        printf("\r\n Connection - In Progress ...\r\n");

        // make your event queue dispatching events forever
        ev_queue.dispatch_forever();

        float temp = htu21df.readTemperature();
        int temp_int = (int)(temp * 100);

        //float humidity = htu21df.readHumidity();
        //int hum_int_htu21df = (int)(humidity * 100);

       // float as6212Temp = as6212.readTemperature();
        //int temp_int_AS6212 = (int)(as6212Temp * 100);

        // float pressure = pressureSensor.readPressure();

        printf("HTU21DF Temperature = %d.%02d C\n", temp_int / 100, temp_int % 100);
        //printf("Humidity = %d.%02d %%\n", hum_int_htu21df / 100, hum_int_htu21df % 100);
        //printf("AS6212 Temperature = %d.%02d C\n", temp_int_AS6212 / 100, temp_int_AS6212 % 100);

        // printf("Pressure = %.2f hPa\n", pressure);

        //ThisThread::sleep_for(500ms);

    return 0;
}
