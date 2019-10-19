
	//loadAllDetectors.h
  //21-02-2019

  #include "crypticontics.h"
  using std
  string name, runFile, filename;
  stringstream momentumString, ampsString, runNumberString;
  TFile file;
  TTree *t = 0;
  //THStack *hs = new THStack("hs","");
  //TH1D *hs;
  //TChain chain("RAWdata");
  int entries;
  double momentum, amps;
  //TOF
  const ULong64_t ntof0=10, ntof1=10;
  UInt_t tof0[ntof0], tof1[ntof1];
  //DWC1
  const ULong64_t ndwc1L = 100, ndwc1R = 100, ndwc1U = 100, ndwc1D = 100;
  UInt_t dwc1L[ndwc1L], dwc1R[ndwc1R], dwc1U[ndwc1U], dwc1D[ndwc1D];
  //DWC2
  const ULong64_t ndwc2L = 100, ndwc2R = 100, ndwc2U = 100, ndwc2D = 100;
  UInt_t dwc2L[ndwc2L], dwc2R[ndwc2R], dwc2U[ndwc2U], dwc2D[ndwc2D];
  //DWC3
  const ULong64_t ndwc3L = 100, ndwc3R = 100, ndwc3U = 100, ndwc3D = 100;
  UInt_t dwc3L[ndwc3L], dwc3R[ndwc3R], dwc3U[ndwc3U], dwc3D[ndwc3D];
  //MicroMegas
  const ULong64_t nmm1 = 1000, nmm2 = 1000, nmm3 = 1000, nmm4 = 1000;
  UInt_t mm1StripQ[nmm1], mm2StripQ[nmm2], mm3StripQ[nmm3], mm4StripQ[nmm4];
  UInt_t mm1_strip_offset = 0, mm2_strip_offset = 0, mm3_strip_offset = 0, mm4_strip_offset = 0;

  void loadAllDetectors(int runNumber, bool text = true){
    
    name="";
    runFile="";
    filename="";
    momentumString = new stringstream;
    ampsString = new stringstream, 
    runNumberString = new stringstream;

    if(!initiated){
      initiateDatabase();
    }
    if(runExists(runNumber)){
      if (getisOK(runNumber)){
        if(file.IsOpen()){
          if(text)
            cout<<"Closing previous file\n";
            file.Close();
        }
        momentum = getMomentum(runNumber)/1000.0;
        amps = getAmps(runNumber);
        momentumString<<momentum;
        ampsString<<amps;
        bool muonFilter=getMuonFilter(runNumber);
        if(!muonFilter){
          name = (momentumString.str()+"GeV/c, "+ampsString.str()+"Amperes");
        } else {
          name = (momentumString.str()+"GeV/c, "+ampsString.str()+"Amperes, Muon Filter");
        }

        beam=Beam(momentum);
        magnet=Magnet("MDX27CO", amps/240);

        runNumberString<<runNumber;
        runFile = (runNumberString.str()+".root");
        filename=("C:/Users/Satchit Chatterji/root_v5.34.36/Deflection/"+runFile);
        if(text){
          cout<<"File named\n";
          cout<<runFile<<endl;
        }
        if(!gSystem->AccessPathName(filename.c_str())){ //if the system can access the file 
          file = new TFile(filename.c_str(),"READ");
          if(text)
            cout<<"File opened\n";

          //get the actual file data
          file.GetObject("RAWdata", t);
          if(text)
            cout<<"RAWdata gotten\n";
          //t->Print();

          //Total entries in file:
          entries = t->GetEntries();
          if(text){
            cout<<runFile<<endl;
            cout << "Entries = " << entries << endl;   
          }
          //setting branch addresses for local use
          //TOF0
          t->SetBranchAddress("NTDC0_ch0",&ntof0);
          t->SetBranchAddress("TDC0_ch0",tof0);
          //TOF1
          t->SetBranchAddress("NTDC1_ch0",&ntof1);
          t->SetBranchAddress("TDC0_ch1",tof1);
          //DWC1 LR
          t->SetBranchAddress("NTDC0_ch8",&ndwc1L);
          t->SetBranchAddress("TDC0_ch8",dwc1L);
          t->SetBranchAddress("NTDC0_ch9",&ndwc1R);
          t->SetBranchAddress("TDC0_ch9",dwc1R);
          //DWC1 UD
          t->SetBranchAddress("NTDC0_ch10",&ndwc1U);
          t->SetBranchAddress("TDC0_ch10",dwc1U);
          t->SetBranchAddress("NTDC0_ch11",&ndwc1D);
          t->SetBranchAddress("TDC0_ch11",dwc1D);
          //DWC2 LR
          t->SetBranchAddress("NTDC0_ch12",&ndwc2L);
          t->SetBranchAddress("TDC0_ch12",dwc2L);
          t->SetBranchAddress("NTDC0_ch13",&ndwc2R);
          t->SetBranchAddress("TDC0_ch13",dwc2R);
          //DWC2 UD
          t->SetBranchAddress("NTDC0_ch14",&ndwc2U);
          t->SetBranchAddress("TDC0_ch14",dwc2U);
          t->SetBranchAddress("NTDC0_ch15",&ndwc2D);
          t->SetBranchAddress("TDC0_ch15",dwc2D);
          //DWC3 LR
          t->SetBranchAddress("NTDC1_ch4",&ndwc3L);
          t->SetBranchAddress("TDC1_ch4",dwc3L);
          t->SetBranchAddress("NTDC1_ch5",&ndwc3R);
          t->SetBranchAddress("TDC1_ch5",dwc3R);
          //DWC3 UD
          t->SetBranchAddress("NTDC1_ch6",&ndwc3U);
          t->SetBranchAddress("TDC1_ch6",dwc3U);
          t->SetBranchAddress("NTDC1_ch7",&ndwc3D);
          t->SetBranchAddress("TDC1_ch7",dwc3D);
          
          if(t->GetListOfBranches()->FindObject("NBL4Smm1_stripQ")){
            //MicroMega1
            t->SetBranchAddress("NBL4Smm1_stripQ",&nmm1);
            t->SetBranchAddress("BL4Smm1_stripQ",mm1StripQ);
            t->SetBranchAddress("BL4Smm1_strip_offset",&mm1_strip_offset);
            //MicroMega3
            t->SetBranchAddress("NBL4Smm3_stripQ",&nmm3);
            t->SetBranchAddress("BL4Smm3_stripQ",mm3StripQ);
            t->SetBranchAddress("BL4Smm3_strip_offset",&mm3_strip_offset); 
            //MicroMega2
            t->SetBranchAddress("NBL4Smm2_stripQ",&nmm2);
            t->SetBranchAddress("BL4Smm2_stripQ",mm2StripQ);
            t->SetBranchAddress("BL4Smm2_strip_offset",&mm2_strip_offset);
            //MicroMega4
            t->SetBranchAddress("NBL4Smm4_stripQ",&nmm4);
            t->SetBranchAddress("BL4Smm4_stripQ",mm4StripQ);
            t->SetBranchAddress("BL4Smm4_strip_offset",&mm4_strip_offset);
          } else {
            if(text){cout<<"No Micromegas in run "<<runNumber<<endl;}
          }
        } else {
          if(text){cout<<"File "<< runNumber <<".root not found in directory\n";}
        } 
      } else {
        if(text){
          cout<<"Run "<<runNumber<<" is not okay"<<endl;
        }
      }
    } else {
      if(text){
        cout<<"No run found with run number "<<runNumber<<endl;
      }
    }
  }
  double timeFactor = 0.025; //needed for signal to time conversion for DWC and TOF

  //TOF Constants
  //double timeOffset = (-8.8552689215);    //source: Swan Code and Links, in ns
  double timeOffset = (-16.32837189507298); //source: Satchit Chatterji, in ns
  //double TOFDistance = 17.0908485684;     //source: Swan Code and Links, in m
  double TOFDistance = (17.067900073686926);//sourec: Satchit Chatterji, in m

  // DWC constants
  double dwc1XSlope = (0.180041184843);  //source: Jorgen Petersen
  double dwc2XSlope = (0.18076370353);   //source: Jorgen Petersen
  double dwc3XSlope = (0.183311);        //source: Kiranbaskar Velmurugan 
  double dwc1XOffset = (0.164959829169); //source: Jorgen Petersen
  double dwc2XOffset = (0.283352915318); //source: Jorgen Petersen
  double dwc3XOffset = (0.148265);       //source: Kiranbaskar Velmurugan
  double dwc1XDisplacement = (0.018);    //source: Jorgen Petersen
  //double dwc1XDisplacement = (9.53325);         //source: Satchit Chatterji
  double dwc2XDisplacement = (0.085);    //source: Jorgen Petersen
  //double dwc2XDisplacement = (12.4822);         //source: Satchit Chatterji
  double dwc3XDisplacement = (0.4);      //assumption

  double dwc1YSlope = (0.181033931267);  //source: Jorgen Petersen
  double dwc2YSlope = (0.184401972456);  //source: Jorgen Petersen
  double dwc3YSlope = (0.183797);        //source: Kiranbaskar Velmurugan
  double dwc1YOffset = (0.207358112687); //source: Jorgen Petersen
  double dwc2YOffset = (0.101045274454); //source: Jorgen Petersen
  double dwc3YOffset = (-0.221857);      //source: Kiranbaskar Velmurugan
  double dwc1YDisplacement = (0.018);    //source: Jorgen Petersen
  double dwc2YDisplacement = (0.085);    //source: Jorgen Petersen
  double dwc3YDisplacement = (0.0);      //assumtion

  //MicroMega Constants
  double pitch = 0.45;     //in mm
  double mmWidth = 460.35; //in mm
  double mm2YDisplacement = -23.1292;  //in mm     //source:Satchit Chatterji
  double mm4YDisplacement = -24.0352;  //in mm     //source:Satchit Chatterji

  //TOF Variables
  double timeDifference;
  //DWC Variables
  double dwc1XPos, dwc2XPos, dwc3XPos, dwc1YPos, dwc2YPos, dwc3YPos;
  //Micromega Variables
  int minhits;
  double QSum1, QSum2, QSum3, QSum4; 
  double indexSum1, indexSum2, indexSum3, indexSum4;
  double clusterIndex1, clusterIndex2, clusterIndex3, clusterIndex4;
  double clusterPosition1, clusterPosition2, clusterPosition3, clusterPosition4; 
