#include "Devices/Concrete/DelsysAnalogDevice.h"

#include <thread>

using namespace STIMWALKER_NAMESPACE::devices;

size_t DELSYS_ANALOG_CHANNEL_COUNT(144);
size_t DELSYS_ANALOG_ACQUISITION_FREQUENCY(148);
std::chrono::microseconds DELSYS_ANALOG_FRAME_RATE(
    1000 * 1000 * 1 / DELSYS_ANALOG_ACQUISITION_FREQUENCY);
size_t DELSYS_ANALOG_SAMPLE_COUNT(4);

DelsysAnalogDevice::DelsysAnalogDevice(const std::string &host, size_t dataPort,
                                       size_t commandPort)
    : DelsysBaseDevice(DELSYS_ANALOG_CHANNEL_COUNT, DELSYS_ANALOG_FRAME_RATE,
                       DELSYS_ANALOG_SAMPLE_COUNT, host, dataPort,
                       commandPort) {}

DelsysAnalogDevice::DelsysAnalogDevice(
    std::unique_ptr<CommandTcpDevice> commandDevice,
    std::unique_ptr<DataTcpDevice> dataDevice)
    : DelsysBaseDevice(std::move(commandDevice), std::move(dataDevice),
                       DELSYS_ANALOG_CHANNEL_COUNT, DELSYS_ANALOG_FRAME_RATE,
                       DELSYS_ANALOG_SAMPLE_COUNT) {}

DelsysAnalogDevice::~DelsysAnalogDevice() {
  stopDataCollectorWorkers();
  stopDeviceWorkers();
}

std::string DelsysAnalogDevice::deviceName() const {
  return "DelsysAnalogDevice";
}

std::string DelsysAnalogDevice::dataCollectorName() const {
  return "DelsysAnalogDataCollector";
}

/// ------------ ///
/// MOCK SECTION ///
/// ------------ ///
using namespace DelsysBaseDeviceMock;
DelsysAnalogDeviceMock::DelsysAnalogDeviceMock(const std::string &host,
                                               size_t dataPort,
                                               size_t commandPort)
    : DelsysAnalogDevice(
          std::make_unique<CommandTcpDeviceMock>(host, commandPort),
          std::make_unique<DataTcpDeviceMock>(
              DELSYS_ANALOG_CHANNEL_COUNT, DELSYS_ANALOG_FRAME_RATE,
              DELSYS_ANALOG_SAMPLE_COUNT, host, dataPort)) {}

bool DelsysAnalogDeviceMock::handleConnect() {
  if (shouldFailToConnect) {
    // Simulate a failure to connect after few time
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return false;
  }
  return DelsysAnalogDevice::handleConnect();
}

bool DelsysAnalogDeviceMock::handleStartDataStreaming() {
  if (shouldFailToStartDataStreaming) {
    // Simulate a failure to connect after few time
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
    return false;
  }
  return DelsysAnalogDevice::handleStartDataStreaming();
}