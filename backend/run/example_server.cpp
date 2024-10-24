#include "stimwalker.h"

#include <asio.hpp>

using namespace STIMWALKER_NAMESPACE;

int main() {
  auto &logger = utils::Logger::getInstance();
  logger.setLogLevel(utils::Logger::INFO);
  logger.info("Starting the application");

  try {
    // Create a TCP server asynchroniously
    server::TcpServerMock server;
    server.startServer();

    // Connect to this server using a TCP client
    server::TcpClient client;
    client.connect();

    // Add the devices
    client.addDelsysDevice();
    client.addMagstimDevice();

    // Start recording data
    // client.startRecording();
    // std::this_thread::sleep_for(std::chrono::seconds(2));
    // client.stopRecording();
    // auto data = client.getLastTrialData();
    // logger.info("One trial received containing: " +
    //             std::to_string(data["DelsysEmgDataCollector"].size()) +
    //             " data series (expected about ~4000)");

    // Remove the only data collector we have
    client.removeMagstimDevice();
    client.startRecording();
    std::this_thread::sleep_for(std::chrono::seconds(6));
    client.stopRecording();
    auto data = client.getLastTrialData();
    logger.info("A second trial received containing: " +
                std::to_string(data["DelsysEmgDataCollector"].size()) +
                " data series (expected about ~" + std::to_string(6 * 2000) +
                ")");

    // Clean up things
    client.disconnect();
    server.stopServer();

  } catch (std::exception &e) {
    logger.fatal(e.what());
    return EXIT_FAILURE;
  }

  logger.info("Exiting the application");
  return EXIT_SUCCESS;
}