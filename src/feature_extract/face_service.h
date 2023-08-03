#pragma once

#include "config.h"
#include "donde/feature_extract/face_pipeline.h"
#include "feature_extract.grpc.pb.h"
#include "feature_extract.pb.h"

// #include "spdlog/spdlog.h"

#include <grpc/grpc.h>
#include <grpcpp/security/server_credentials.h>
#include <grpcpp/server.h>
#include <grpcpp/server_builder.h>
#include <grpcpp/server_context.h>
#include <iostream>

using namespace std;

using donde_toolkits::feature_extract::FacePipeline;

using com::sekirocc::feature_extract::CompareRequest;
using com::sekirocc::feature_extract::CompareResponse;
using com::sekirocc::feature_extract::DetectionRequest;
using com::sekirocc::feature_extract::DetectionResponse;
using com::sekirocc::feature_extract::ExtractionRequest;
using com::sekirocc::feature_extract::ExtractionResponse;
using com::sekirocc::feature_extract::FaceService;

using grpc::ServerContext;
using grpc::Status;

class FaceServiceImpl final : public FaceService::Service {
  public:
    FaceServiceImpl(Config& server_config);
    ~FaceServiceImpl();

    void Start();
    void Stop();

    Status Detect(ServerContext* context, const DetectionRequest* request,
                  DetectionResponse* response) override;

    Status ExtractFeature(ServerContext* context, const ExtractionRequest* request,
                          ExtractionResponse* response) override;

    Status CompareFeature(ServerContext* context, const CompareRequest* request,
                          CompareResponse* response) override;

  private:
    Config& config;
    // spdlog::Logger& logger;
    std::string device_id;
    FacePipeline pipeline;
};
