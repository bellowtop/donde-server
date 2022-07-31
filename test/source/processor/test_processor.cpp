#include "concurrent_processor.h"
#include "types.h"

#include <Poco/Logger.h>
#include <Poco/NotificationQueue.h>
#include <doctest/doctest.h>
#include <memory>
#include <nlohmann/json.hpp>

using namespace std;
using namespace Poco;

using Poco::Logger;
using Poco::Notification;
using Poco::NotificationQueue;

using nlohmann::json;

TEST_CASE("ConcurrentProcessor comunicate with DummyWorker using channel.") {
    static int processedMsg = 0;

    //
    // define a dummy worker for this test.
    //
    class DummyWorker : public Worker {
      public:
        DummyWorker(std::shared_ptr<NotificationQueue> ch) : Worker(ch){};
        ~DummyWorker(){};

        RetCode Init(json conf, int id, std::string device_id) override {
            CHECK(conf != nullptr);
            CHECK(id != -1);
            CHECK(device_id == "-1");
            return RET_OK;
        };

        void run() override {
            for (;;) {
                Notification::Ptr pNf(_channel->waitDequeueNotification());

                if (pNf) {
                    WorkMessage::Ptr msg = pNf.cast<WorkMessage>();
                    if (msg) {
                        if (msg->isQuitMessage()) {
                            break;
                        }
                        processedMsg ++;

                        Value input = msg->getRequest();
                        std::cout << "get input, valueType: " << input.valueType << std::endl;

                        CHECK(input.valueType == ValueFrame);
                        CHECK(input.valuePtr != nullptr);

                        // allocate memory in heap, the caller is responsible to free it!
                        Feature *result = new Feature;
                        result->data.resize(100);
                        Value output{ValueFeature, result};

                        msg->setResponse(output);
                    }
                } else {
                    break;
                }
            }
        };

      private:
        Logger& _logger = Logger::get("DummyWorker");
    };

    //
    // test it.
    //

    std::string config = "{}";
    int concurrent = 3;
    std::string device_id = "-1";

    ConcurrentProcessor<DummyWorker> processor{config, concurrent, device_id};

    json conf = {
        {"pipeline", {}}
    };
    processor.Init(conf);

    Frame f;
    Value input{ValueFrame, &f};

    Value output;
    processor.Process(input, output);

    CHECK(output.valueType == ValueFeature);
    CHECK(output.valuePtr != nullptr);

    // you must release output pointer!
    Feature* feature = (Feature*) output.valuePtr;
    CHECK(feature->data.size() == 100);
    delete feature;

    processor.Process(input, output);
    processor.Process(input, output);
    // Process is  blocking function, it will wait for output.
    CHECK(processedMsg == 3);

    processor.Terminate();

    CHECK("aa" == "aa");
};
