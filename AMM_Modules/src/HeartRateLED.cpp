#include "stdafx.h"

#include "AMM/DDS_Manager.h"

#include <sys/ioctl.h>
#include <linux/types.h>
#include <linux/spi/spidev.h>

#include <fcntl.h>    /* For O_RDWR */

using namespace std;

const char *tourniquet_action = "PROPER_TOURNIQUET";
const char *hemorrhage_action = "LEG_HEMORRHAGE";
float heartrate = 40.0;
float breathrate = 12.0;
bool tourniquet = false;
bool hemorrhage = false;
bool closed = false;

static const char *device = "/dev/spidev0.0";
static uint8_t mode;
static uint8_t bits = 8;
static uint32_t speed = 1 << 23;
static uint16_t delay;

int spi_transfer(int fd, unsigned char *tx_buf, unsigned char *rx_buf, int buflen) {
    int ret;
    struct spi_ioc_transfer tr = {tx_buf : (unsigned long) tx_buf, rx_buf : (unsigned long) rx_buf, len : buflen, speed_hz : speed,
            delay_usecs : delay, bits_per_word : bits,};

    ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
    if (ret < 1)
        perror("can't send spi message");
    return ret;
}


class HeartRateListener : public ListenerInterface {

    void onNewNodeData(AMM::Physiology::Node n) {
        bool print = false;
        if (n.dbl() == -1.0f) {
            closed = true;
            return;
        }

        if (n.nodepath().compare("Cardiovascular_HeartRate") == 0) {
            heartrate = n.dbl();
            print = true;
        }

        if (n.nodepath().compare("Respiratory_Respiration_Rate") == 0) {
            breathrate = n.dbl();
            print = true;
        }

        if (print) {
            cout << "=== [HeartRateLED] Received data :  ("
                 << n.nodepath() << ", " << n.dbl() << ')'
                 << endl;
        }
    }

    void onNewCommandData(AMM::PatientAction::BioGears::Command c) {
        if (c.message().compare(tourniquet_action) == 0) {
            tourniquet = true;
        }
        if (c.message().compare(hemorrhage_action) == 0) {
            hemorrhage = true;
        }
    }
};

int main(int argc, char *argv[]) {

    int spi_fd = open(device, O_RDWR);

    auto *mgr = new DDS_Manager();
    auto *pub_listener = new DDS_Listeners::PubListener();
    auto *node_sub_listener = new DDS_Listeners::NodeSubListener();
    auto *command_sub_listener = new DDS_Listeners::CommandSubListener();

    HeartRateListener vel;
    node_sub_listener->SetUpstream(&vel);
    command_sub_listener->SetUpstream(&vel);

    Publisher *command_publisher = mgr->InitializeCommandPublisher(pub_listener);

    Subscriber *node_subscriber = mgr->InitializeNodeSubscriber(node_sub_listener);
    Subscriber *command_subscriber = mgr->InitializeCommandSubscriber(command_sub_listener);

    cout << "=== [HeartRateLED] Ready ..." << endl;


    int count = 0;
    while (!closed) {
        //prepare SPI message
        /*
         heartrate = 60 (Example)
         heartrate/60 = 1 = beats/second
         seconds/ms = 1/1000
         want ms/beat
         beats/second*seconds/ms = beats/ms
         1/beats/ms = ms/beat

         answer = 1/(beats/min * min/sec * sec/ms)
         answer = 1/(hr * (1/60) * 0.001)
         */
        //int spi_msg_full = 1.0/(heartrate * (1.0/60.0) * 0.001);
        unsigned char spi_send[4];
        spi_send[0] = heartrate;
        spi_send[1] = breathrate;
        spi_send[2] = tourniquet;
        spi_send[3] = hemorrhage;
        unsigned char spi_rcvd[4];

        //do SPI communication
        int spi_tr_res = spi_transfer(spi_fd, spi_send, spi_rcvd, 4);

        //std::cout << "spi_msg " << std::hex << std::setw(2)
        //	<< std::setfill('0') << (unsigned int) spi_msg << std::endl;
        //std::cout << "spi_rcvd " << std::hex << std::setw(2)
        //	<< std::setfill('0') << (unsigned int) spi_rcvd << std::endl;
        //send press messages based on received SPI
        //the buttons send 1 when they are up and 0 when they are pressed
        if (spi_rcvd[1]) {
            //button 2 was pressed
            //send hemorrhage action
            AMM::PatientAction::BioGears::Command cmdInstance;
            cmdInstance.message(hemorrhage_action);
            cout << "=== [CommandExecutor] Sending a command containing:" << endl;
            cout << "    Command : \"" << cmdInstance.message() << "\"" << endl;
            command_publisher->write(&cmdInstance);
            cout << "sent that command" << endl;
        }


        ++count;
    }

    cout << "=== [HeartRateLED] Simulation stopped." << endl;


    return 0;

}
