#include "ModuleManager.h"

using namespace std;
using namespace eprosima;
using namespace eprosima::fastrtps;
using namespace sqlite;

namespace AMM {
    ModuleManager::ModuleManager() {
        auto *status_sub_listener = new DDS_Listeners::StatusSubListener();
        auto *config_sub_listener = new DDS_Listeners::ConfigSubListener();

        status_sub_listener->SetUpstream(this);
        config_sub_listener->SetUpstream(this);

        mgr->InitializeReliableSubscriber(AMM::DataTypes::statusTopic, AMM::DataTypes::getStatusType(), status_sub_listener);
        mgr->InitializeReliableSubscriber(AMM::DataTypes::configurationTopic, AMM::DataTypes::getConfigurationType(),
                                  config_sub_listener);

        currentScenario = mgr->GetScenario();
        m_runThread = false;
    }

    bool ModuleManager::isRunning() {
        return m_runThread;
    }

    void ModuleManager::Start() {
        std::string nodeString(nodeName);
        // Publish module configuration once we've set all our publishers and listeners
        // This announces that we're available for configuration
        mgr->PublishModuleConfiguration(
                mgr->module_id,
                nodeString,
                "Vcom3D",
                "Module_Manager",
                "00001",
                "0.0.1",
                mgr->GetCapabilitiesAsString("mule1/module_capabilities/module_manager_capabilities.xml")
        );

        mgr->SetStatus(mgr->module_id, nodeString, OPERATIONAL);

        if (!m_runThread) {
            m_runThread = true;
	    //            m_thread = std::thread(&ModuleManager::RunLoop, this);

        }
    }

    void ModuleManager::RunLoop() {
        while (m_runThread) {
	  //            m_mutex.lock();
	  //	  std::this_thread::sleep_for(std::chrono::milliseconds(50));
	  // do work
	  //            m_mutex.unlock();
        }
    }

    void ModuleManager::ShowStatus() {

    }

    void ModuleManager::Cleanup() {

    }

    void ModuleManager::Shutdown() {
        if (m_runThread) {
            m_runThread = false;
            std::this_thread::sleep_for(std::chrono::milliseconds(200));
	    //            m_thread.join();
        }

        Cleanup();

    }

    void ModuleManager::onNewStatusData(AMM::Capability::Status st, SampleInfo_t *info)  {
        ostringstream statusValue;
        statusValue << st.status_value();


        LOG_TRACE << "[" << st.module_id() << "][" << st.module_name() << "][" << st.capability() << "] Status = " << statusValue.str();

	        GUID_t changeGuid = info->sample_identity.writer_guid();
        std::ostringstream module_guid;
        module_guid << changeGuid;


	try {
            database db("amm.db");
            db << "replace into module_status (module_id, module_guid, module_name, capability, status) values (?,?,?,?,?);"
               << st.module_id()
               << module_guid.str()
               << st.module_name()
               << st.capability()
               << statusValue.str();
        } catch (exception &e) {
            LOG_ERROR << "[ModuleManager][STATUS]" << e.what();
        }
    }

    void ModuleManager::onNewConfigData(AMM::Capability::Configuration cfg, SampleInfo_t *info) {

        LOG_TRACE << "[" << cfg.module_id() << "][" << cfg.module_name() << "] sent capabilities";

        GUID_t changeGuid = info->sample_identity.writer_guid();
        std::ostringstream module_guid;
        module_guid << changeGuid;

	
        try {
            database db("amm.db");
            db
                    << "replace into module_capabilities (module_id, module_guid, module_name, manufacturer, model, serial_number, version, capabilities) values (?,?,?,?,?,?,?,?);"
                    << cfg.module_id()
                    << module_guid.str()
                    << cfg.module_name()
                    << cfg.manufacturer()
                    << cfg.model()
                    << cfg.serial_number()
                    << cfg.version()
                    << cfg.capabilities();
        } catch (exception &e) {
            LOG_ERROR << "[ModuleManager][CONFIG]" << e.what();
        };

    }
}
