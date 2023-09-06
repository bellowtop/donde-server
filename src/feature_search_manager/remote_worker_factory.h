
#include "donde/feature_search/worker.h"

#include <donde/feature_search/search_manager/worker_factory.h>

using namespace donde_toolkits::feature_search::search_manager;

class RemoteWorkerFactory : public WorkerFactory {

    RemoteWorkerFactory();
    static Worker* CreateWorker(const std::string& worker_id, const std::string& worker_address);
    static bool ProbeWorker(const std::string& worker_address);

    ~RemoteWorkerFactory() = default;
};
