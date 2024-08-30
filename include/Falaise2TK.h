#ifndef FALAISE2TK
#define FALAISE2TK

// Interface from Falaise
#include "bayeux/dpp/base_module.h"
#include "bayeux/mctools/simulated_data.h"

// Third party:
// - Bayeux/geomtools:
#include <bayeux/geomtools/line_3d.h>
#include <bayeux/geomtools/polyline_3d.h>

#include "falaise/snemo/processing/module.h"
#include "falaise/snemo/datamodels/calibrated_data.h"
#include <falaise/snemo/datamodels/data_model.h>
#include <falaise/snemo/datamodels/event_header.h>
#include <falaise/snemo/datamodels/tracker_clustering_data.h>
#include "falaise/snemo/datamodels/tracker_clustering_solution.h"
#include <falaise/snemo/datamodels/tracker_trajectory_data.h>
#include <falaise/snemo/datamodels/tracker_trajectory_solution.h>
#include <falaise/snemo/datamodels/line_trajectory_pattern.h>
#include <falaise/snemo/datamodels/polyline_trajectory_pattern.h>
#include "falaise/snemo/datamodels/particle_track_data.h"

#include "TKEvent.h"

// ROOT headers
#include "TTree.h"
#include "TFile.h"

class Falaise2TK : public dpp::base_module
{
public:
	////////////////////////////////////////////////
	// The following PUBLIC methods MUST be defined!
	// Default constructor
	Falaise2TK();

	// Default destructor
	virtual ~Falaise2TK();

	//! Configure the module
	virtual void initialize(
	const datatools::properties &myConfig,
	datatools::service_manager &flServices,
	dpp::module_handle_dict_type &what);

	//! Reset the module
	virtual void reset();

	// Process event
	virtual dpp::base_module::process_status process(datatools::things &workItem);

	TKEvent* get_event_data(datatools::things &workItem);


private:

	TTree* tree;
	TFile* file;
	TKEvent* event;
	DPP_MODULE_REGISTRATION_INTERFACE(Falaise2TK);
};

#endif // FALAISE2TK
