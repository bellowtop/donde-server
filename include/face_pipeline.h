#pragma once

#include "Poco/Logger.h"
#include "concurrent_processor.h"
#include "detector_worker.h"
#include "types.h"

#include <map>

using namespace Poco;
using namespace std;

class FacePipeline {
  public:
    FacePipeline(const std::string config, const std::string device_id);

    RetCode Init(std::shared_ptr<Processor> detectorProcessor,
                 std::shared_ptr<Processor> alignerProcessor,
                 std::shared_ptr<Processor> landmarksProcessor,
                 std::shared_ptr<Processor> featureProcessor);

    RetCode Terminate();

    Frame Decode(const vector<uint8_t>& image_data);
    DetectResult Detect(const Frame& frame);

  private:
    std::string _config;
    std::string _device_id;
    Poco::Logger& _logger;

    std::shared_ptr<Processor> _detectorProcessor;
    std::shared_ptr<Processor> _landmarksProcessor;
    std::shared_ptr<Processor> _alignerProcessor;
    std::shared_ptr<Processor> _featureProcessor;
};
