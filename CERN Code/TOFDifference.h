//TOFDifference.h
//17-04-2019

TLine protonLine, electronLine;
TLine cLine;
double getTOF(){
	return (double(tof1[0]-tof0[0])*timeFactor-timeOffset)*1e-9;
}
double tofSpeed(){
	return TOFDistance/getTOF();
}
void drawTOF(bool drawCLine = false){
	TLine protonLine, electronLine;
	tofCanvas = new TCanvas("tofCanvas","tofCanvas",750,600);
	//string tofname = ("Run "+runNumberString.str()+", "+momentumString.str()+"GeV/c");
	string tofname = (momentumString.str()+"GeV/c");
	TOFHist = new TH1D(tofname.c_str(), "TOF of Particles;nanoseconds;Count", 200, 40e-9, 0.075e-6);
	for(int i = 0; i < entries; i++){
		t->GetEntry(i);			
		TOFHist->Fill(getTOF());
	}
	TOFHist->Draw();

	gStyle->SetOptStat("ne");
	protonLine = TLine(fabs(TOFDistance/beam.proton.speed),0,fabs(TOFDistance/beam.proton.speed),TOFHist->GetMaximum()+TOFHist->GetMaximum()/20);
	protonLine.SetLineColor(kRed);
	protonLine.Draw();

	electronLine = TLine(fabs(TOFDistance/beam.positron.speed),0,fabs(TOFDistance/beam.positron.speed),TOFHist->GetMaximum()+TOFHist->GetMaximum()/20);
	electronLine.SetLineColor(kOrange);
	electronLine.Draw();

	if(drawCLine){
		cLine = TLine(fabs(TOFDistance/c),0,fabs(TOFDistance/c),TOFHist->GetMaximum()+TOFHist->GetMaximum()/20);
		cLine.SetLineColor(kBlack);
		cLine.Draw();
		cout<<"Black Line indicates theoretical photon TOF.\n";	
	}
	TOFHist->SetDirectory(0);
	cout<<"Red Line indicates theoretical proton/antiproton TOF.\n";
	cout<<"Yellow Line indicates theoretical electron/positron TOF.\n";
}
void drawTOFSpeed(bool drawCLine = false){
	tofCanvas2 = new TCanvas("tofCanvas2","tofCanvas2",750,600);
	string tofname = ("Run "+runNumberString.str()+", "+momentumString.str()+"GeV/c");
	TOFHist = new TH1D(tofname.c_str(), "Speed of Particles;Fraction of c;Count", 201, 0.25,1.5);
	for(int i = 0; i < entries; i++){
		t->GetEntry(i);			
		TOFHist->Fill(tofSpeed()/c);
	}
	TOFHist->Draw();

	protonLine=TLine(fabs(beam.proton.speed/c),0,fabs(beam.proton.speed/c),TOFHist->GetMaximum()+TOFHist->GetMaximum()/20);
	protonLine.SetLineColor(kRed);
	protonLine.Draw();

	electronLine=TLine(fabs(beam.positron.speed/c),0,fabs(beam.positron.speed/c),TOFHist->GetMaximum()+TOFHist->GetMaximum()/20);
	electronLine.SetLineColor(kOrange);
	electronLine.Draw();
	
	if(drawCLine){
		cLine = TLine(fabs(1),0,fabs(1),TOFHist->GetMaximum()+TOFHist->GetMaximum()/20);
		cLine.SetLineColor(kBlack);
		cLine.Draw();
		cout<<"Black Line indicates theoretical photon speed.\n";		
	}
	TOFHist->SetDirectory(0);
	cout<<"Red Line indicates theoretical proton/antiproton speed.\n";
	cout<<"Yellow Line indicates theoretical electron/positron speed.\n";
}