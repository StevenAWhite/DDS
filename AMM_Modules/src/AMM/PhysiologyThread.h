#pragma once

#include <mutex>
#include <thread>
#include <ctime>
#include <stdexcept>
#include <sstream>

// Boost dependencies
#include <boost/assign/std/vector.hpp>
#include <boost/assign/list_of.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

// BioGears core
#include <biogears/cdm/CommonDataModel.h>
#include <biogears/engine/BioGearsPhysiologyEngine.h>

#include <biogears/cdm/patient/SEPatient.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/compartment/fluid/SEGasCompartment.h>
#include <biogears/cdm/compartment/fluid/SELiquidCompartment.h>
#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>
#include <biogears/cdm/system/physiology/SERespiratorySystem.h>

#include <biogears/cdm/compartment/SECompartmentManager.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>

#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/system/physiology/SEBloodChemistrySystem.h>
#include <biogears/cdm/system/physiology/SECardiovascularSystem.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/system/physiology/SENervousSystem.h>

#include <biogears/cdm/substance/SESubstanceManager.h>
#include <biogears/cdm/substance/SESubstance.h>

#include <biogears/cdm/utils/SEEventHandler.h>
#include <biogears/cdm/engine/PhysiologyEngineTrack.h>

#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>
#include <biogears/cdm/properties/SEFunctionVolumeVsTime.h>
#include <biogears/cdm/properties/SEScalarMass.h>
#include <biogears/cdm/properties/SEScalarLength.h>
#include <biogears/cdm/properties/SEScalarAmountPerVolume.h>
#include <biogears/cdm/properties/SEScalarFraction.h>
#include <biogears/cdm/properties/SEScalarFrequency.h>
#include <biogears/cdm/properties/SEScalarMassPerVolume.h>
#include <biogears/cdm/properties/SEScalarPressure.h>
#include <biogears/cdm/properties/SEScalarTemperature.h>
#include <biogears/cdm/properties/SEScalarTime.h>
#include <biogears/cdm/properties/SEScalarVolume.h>
#include <biogears/cdm/properties/SEScalarVolumePerTime.h>

#include <biogears/cdm/patient/assessments/SEComprehensiveMetabolicPanel.h>
#include <biogears/cdm/patient/assessments/SEPulmonaryFunctionTest.h>
#include <biogears/cdm/patient/assessments/SECompleteBloodCount.h>
#include <biogears/cdm/patient/actions/SESubstanceBolus.h>
#include <biogears/cdm/patient/actions/SESubstanceInfusion.h>
#include <biogears/cdm/patient/actions/SESubstanceCompoundInfusion.h>
#include <biogears/cdm/substance/SESubstanceCompound.h>
#include <biogears/cdm/patient/actions/SEPainStimulus.h>
#include <biogears/cdm/system/physiology/SEDrugSystem.h>
#include <biogears/cdm/system/physiology/SEEnergySystem.h>

#include <biogears/cdm/system/equipment/ElectroCardioGram/SEElectroCardioGram.h>

#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachine.h>
#include <biogears/cdm/system/equipment/Anesthesia/SEAnesthesiaMachineOxygenBottle.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEAnesthesiaMachineConfiguration.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEMaskLeak.h>
#include <biogears/cdm/system/equipment/Anesthesia/actions/SEOxygenWallPortPressureLoss.h>

#include <biogears/cdm/scenario/SEScenario.h>
#include <biogears/cdm/scenario/SEScenarioExec.h>
#include <biogears/cdm/scenario/SEAdvanceTime.h>

#include "AMMPubSubTypes.h"

#include "AMM/BaseLogger.h"

// #include <boost/stacktrace.hpp>
#include <boost/exception/all.hpp>

// typedef boost::error_info<struct tag_stacktrace, boost::stacktrace::stacktrace> traced;

namespace biogears{
// Forward declare what we will use in our thread
class SESubstance;

class SEEnergySystem;

class SEPainStimulus;

class SEScenario;

class SEAdvanceTime;

class SEComprehensiveMetabolicPanel;

class SEGasCompartment;

class SECompleteBloodCount;

class SEAnesthesiaMachineConfiguration;

class SEAnesthesiaMachine;

class SEHemorrhage;

class SESubstanceCompoundInfusion;

class PhysiologyEngine;
}

namespace AMM {
    class PhysiologyThread {
    public:
        PhysiologyThread(const std::string &stateFile);

        ~PhysiologyThread();

        bool LoadState(const std::string &stateFile, double sec);

        bool SaveState(const std::string &stateFile);

        bool SaveState();

        bool ExecuteXMLCommand(const std::string &cmd);

        bool ExecuteCommand(const std::string &cmd);

        void Shutdown();

        void StartSimulation();

        void StopSimulation();

        void AdvanceTimeTick();

        double GetSimulationTime();

        std::map<std::string, double (PhysiologyThread::*)()> *GetNodePathTable();

        double GetNodePath(const std::string &nodePath);

        void SetVentilator(const std::string &ventilatorSettings);

        void SetIVPump(const std::string &pumpSettings);

        void SetPain(const std::string &painSettings);

        void Status();

        static std::map<std::string, double (PhysiologyThread::*)()> nodePathTable;
        static std::vector<std::string> highFrequencyNodes;

        bool logging_enabled = false;

        std::string getTimestampedFilename(const std::string &basePathname);

    private:
        bool LoadScenarioFile(const std::string &scenarioFile);

        void PopulateNodePathTable();

        void PreloadSubstances();

        void PreloadCompartments();

        double GetShutdownMessage();

        double GetHeartRate();

        double GetBloodVolume();

        double GetArterialSystolicPressure();

        double GetArterialDiastolicPressure();

        double GetMeanArterialPressure();

        double GetArterialPressure();

        double GetMeanCentralVenousPressure();

        double GetEndTidalCarbonDioxideFraction();

        double GetOxygenSaturation();

        double GetRespirationRate();

        double GetCoreTemperature();

        double GetECGWaveform();

        double GetSodiumConcentration();

        double GetSodium();

        double GetGlucoseConcentration();

        double GetBUN();

        double GetCreatinineConcentration();

        double GetWhiteBloodCellCount();

        double GetRedBloodCellCount();

        double GetHemoglobinConcentration();

        double GetHematocrit();

        double GetBloodPH();

        double GetArterialCarbonDioxidePressure();

        double GetArterialOxygenPressure();

        double GetVenousOxygenPressure();

        double GetVenousCarbonDioxidePressure();

        double GetBicarbonateConcentration();

        double GetBicarbonate();

        double GetBaseExcess();

        double GetCO2();

        double GetPotassium();

        double GetChloride();

        double GetPlateletCount();

        double GetExhaledCO2();

        double GetTidalVolume();

        double GetTotalLungVolume();

        double GetLeftLungVolume();

        double GetRightLungVolume();

        double GetLeftPleuralCavityVolume();

        double GetRightPleuralCavityVolume();

        double GetLeftAlveoliBaselineCompliance();

        double GetRightAlveoliBaselineCompliance();

        double GetCardiacOutput();

        double GetCalciumConcentration();

        double GetAlbuminConcentration();

        double GetLactateConcentration();

        double GetTotalBilirubin();

        double GetTotalProtein();

        double GetPainVisualAnalogueScale();

	biogears::SESubstance *sodium;
        biogears::SESubstance *glucose;
        biogears::SESubstance *creatinine;
        biogears::SESubstance *calcium;
        biogears::SESubstance *albumin;
        biogears::SESubstance *hemoglobin;
        biogears::SESubstance *bicarbonate;
        biogears::SESubstance *CO2;
        biogears::SESubstance *N2;
        biogears::SESubstance *O2;
        biogears::SESubstance *CO;
        biogears::SESubstance *potassium;
        biogears::SESubstance *chloride;
        biogears::SESubstance *lactate;

        const biogears::SEGasCompartment *carina;
        const biogears::SEGasCompartment *leftLung;
        const biogears::SEGasCompartment *rightLung;


    protected:
        std::mutex m_mutex;
        bool m_runThread;
        std::unique_ptr<biogears::PhysiologyEngine> m_pe;
    };
}
