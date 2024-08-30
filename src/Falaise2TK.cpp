// Interface from Falaise
#include "Falaise2TK.h"

DPP_MODULE_REGISTRATION_IMPLEMENT(Falaise2TK, "Falaise2TK")

Falaise2TK::Falaise2TK() : dpp::base_module() 
{
	std::cout << "constructed!" << std::endl;
}

Falaise2TK::~Falaise2TK()
{ 
	file->cd();
    	tree->Write();
	file->Close();	
	if (this->is_initialized())
	{
		this->reset();
	}
}

void Falaise2TK::initialize( 
            const datatools::properties&   myConfig,
            datatools::service_manager&    flServices,
            dpp::module_handle_dict_type&  /*moduleDict*/
        ) 
{
	file = new TFile(Form("./runs/Run-%d.root", 0), "RECREATE");
	tree = new TTree("Event", "All data from event");
	event = new TKEvent();
	tree->Branch("Eventdata", &event);	
	this->_set_initialized(true);
}

dpp::base_module::process_status Falaise2TK::process(datatools::things& workItem) 
{
	event = Falaise2TK::get_event_data(workItem); 
	tree->Fill();	
	delete event;
	return falaise::processing::status::PROCESS_OK;
}

void Falaise2TK::reset() 
{   
	this->_set_initialized(false);
}

TKEvent* Falaise2TK::get_event_data(datatools::things& workItem)
{
	auto &header = workItem.get<snemo::datamodel::event_header>("EH");

	TKEvent* event = new TKEvent(header.get_id().get_run_number(), header.get_id().get_event_number());

	if(workItem.has("CD"))
	{
		using namespace snemo::datamodel;

		calibrated_data falaiseCDbank = workItem.get<calibrated_data>("CD");

		for ( auto &calohit : falaiseCDbank.calorimeter_hits() )
		{
			int SWCR[4] = {-1,-1,-1,-1};
			switch( calohit->get_geom_id().get_type() )
			{
			case 1302: 
				SWCR[0] = calohit->get_geom_id().get(1);
				SWCR[2] = calohit->get_geom_id().get(2);
				SWCR[3] = calohit->get_geom_id().get(3);
				break;
			case 1232:
				SWCR[0] = calohit->get_geom_id().get(1);
				SWCR[1] = calohit->get_geom_id().get(2);
				SWCR[2] = calohit->get_geom_id().get(3);
				SWCR[3] = calohit->get_geom_id().get(4);
				break;
			case 1252:
				SWCR[0] = calohit->get_geom_id().get(1);
				SWCR[1] = calohit->get_geom_id().get(2);
				SWCR[2] = calohit->get_geom_id().get(3);
				break;
			}
			TKOMhit* OMhit = new TKOMhit(SWCR);		
			//OMhit->set_energy( calohit->get_energy() ); 
			
			event->get_OM_hits().push_back( OMhit );
		}

		for ( auto &trhit : falaiseCDbank.tracker_hits() )
		{
			int SRL[3] = {trhit->get_side(), trhit->get_row(), trhit->get_layer()};
			TKtrhit* hit = new TKtrhit(SRL);
			
			if( trhit->get_r() == trhit->get_r() )
			{
				hit->set_r( trhit->get_r() );        	
				hit->set_sigma_R( /*trhit->get_sigma_r()*/ 2.0 );
			}
			else
			{
				hit->set_r( -1 );        	
				hit->set_sigma_R( -1 );
			}
			
			if( trhit->get_z() == trhit->get_z() )
			{
				hit->set_h( trhit->get_z() );
		  		hit->set_sigma_Z( trhit->get_sigma_z() );
			}
			else
			{
				hit->set_h( 0 );
			}
			/*
			std::cout << "r: 	" << trhit->get_r() << std::endl;
			std::cout << "sigma_r: 	" << trhit->get_sigma_r() << std::endl;
			std::cout << "Z: 	" << trhit->get_z() << std::endl;
			std::cout << "sigma_Z: 	" << trhit->get_sigma_z() << std::endl;
			*/
			event->get_tr_hits().push_back( hit );
		}

	}
	return event;
}
