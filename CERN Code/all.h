// beamline, crypticontics, runDatabaseArrays, loadAllDetectors,
// trackerScatter, TOFDifference, AODGraphs, getAccVectors.
// Long ago, the eight header files lived together in harmony. 
// Then, everything changed when the ROOT Framework on Macintosh attacked. 
// Only all.h, master of all eight files, could stop them, but when the world needed him most, 
// he didnt exist.
// Five months passed and my brother and I discovered the new all.h, a header file named all.h, 
// and although its analysis skills are great, it has a lot to learn before he's 
// ready to save anyone. But I believe all.h can save BL4S.

//all.h
//14-03-2019
//13th January 2019   

#include <math.h>
#include <string.h>
#pragma once
// #include <iostream>
// using namespace std;
#define POSITIVE true
#define NEGATIVE false

const double pi = 3.14159265358979323846264338328;
const double c = 299792458.0;
const double q = 1.60217662*pow(10,-19);
const int noOfParticles = 6;

class Magnet;
class Mass;
class Particle;
class Momentum;
class Beam;
class Detector;

class Magnet{
    public:
    double strength;
    double width;
    double distance;
    Magnet(){}
    Magnet(double b){
        strength = b; 
    }
    Magnet(char constantM[], double b){
        if(!strcmp(constantM,"MNP17") || !strcmp(constantM,"mnp17")){
            width = 0.40;
            strength = b;
        }
        else if(!strcmp(constantM,"MDX27") || !strcmp(constantM,"mdx27")){
            width = 0.55;
            strength = b;
        }
        else if(!strcmp(constantM,"MDX27CO") || !strcmp(constantM,"mdx27co")){ //Just for Cryptic Ontics 2018
            width = 0.51;
            strength = b;
            distance = 6.236;
        }
        else{
            cout<<"No magnet in file called "<<constantM<<endl;
        }
    }
    void updateData(char type[], double data){
        if(!strcmp(type, "strength")){
            strength = data;
        }
        else if(!strcmp(type, "width")){
            width = data;
        }
        else if(!strcmp(type, "distance")){
            distance = data;
        }
        else {
            cout<<"No member called "<<type<<endl;
        }
    }
    void display(){
        cout<<"Strength: "<<strength<<"T"<<endl;
        cout<<"Width:    "<<width<<"m"<<endl;
        cout<<"Distance: "<<distance<<"m"<<endl;
    }
    ClassDef(Magnet, 1);
};

class Detector{
    public:
    double leastCount;
    double distance;
    Detector(){}
    Detector(double d){
        distance = d;
    }
    Detector(char constantM[], double d){
        if(!strcmp(constantM, "MicroMega") || !strcmp(constantM, "micromega")){
            leastCount = 0.0003;
            distance = d;
        }
        else if(!strcmp(constantM,"DWC") || !strcmp(constantM,"dwc")){
            leastCount = 0.00045;
            distance = d;
        }
        else {
            cout<<"No detector in file called "<<constantM<<endl;
        }
    }
    void updateData(char type[], double data){
        if(!strcmp(type, "least count")){
            leastCount = data;
        }
        if(!strcmp(type, "distance")){
            leastCount = data;
        }
        else {
            cout<<"No member called "<<type<<endl;
        }
    }
    void display(){
        cout<<"Distance:    "<<distance<<"m"<<endl;
        cout<<"Least Count: "<<leastCount<<"m"<<endl;
    }

    ClassDef(Detector, 1);
};

class Mass{
    public:
    double rest;
    double relativistic;
    Mass(){}
    Mass(double m){
        rest = m;
    }
    Mass(double m, double v){
        rest = m;
        relativistic = (rest/sqrt(1-((v*v)/(::c*::c))));
    }
    changeToRelativistic(double v){
        relativistic = (rest/sqrt(1-((v*v)/(::c*::c))));   
    }
    ClassDef(Mass, 1);  
};

class Particle{
    public:
    Particle(){}
    char name[20];
    Mass mass;
    double charge;
    double spin;
    double speed;
    Particle(char n[], double m, double q, double s){
        strcpy(name, n);
        mass = Mass(m);
        charge = q;
        spin = s;
    }
    void assignSpeed(double s){
        speed = s;
        mass.changeToRelativistic(speed);
    }
    void display(){
        cout<<"    Name:   "<<name<<endl;
        cout<<"    Mass:   "<<mass.rest<<endl;
        cout<<"    Charge: "<<charge<<endl;
        cout<<"    Spin:   "<<spin<<endl;
    }
    ClassDef(Particle, 1);
};

class Momentum{
    public:
    Mass mass;
    double velocity;
    double relativistic;
    double SI;
    Momentum(){}
    Momentum(double r){
        relativistic = r;
        changeToSI();
    }
    Momentum(Mass m, double v){
        mass = Mass(m);
        velocity = v;
        SI = m.relativistic*v;
        changeToGeV();
    }
    void changeToSI(){
        SI = relativistic*5.36*pow(10,-19);
    }
    void changeToGeV(){
        relativistic = SI/5.36*pow(10,-19);
    }
    void updateData(char type[], double data){
        if(!strcmp(type, "SI") || !strcmp(type, "si")){
            SI = data;
            changeToGeV();
        }
        else if(!strcmp(type, "relativistic")){
            relativistic = data;
            changeToSI();
        }
        else {
            cout<<"No member called "<<type<<endl;
        }
    }
    void display(){
        cout<<"Momentum: "<<relativistic<<" GeV/c"<<endl;
        cout<<"Momentum: "<<SI<<"kgm/s"<<endl;
    }
    ClassDef(Momentum, 1);
};

class Beam{
    public:
    Momentum momentum;
    bool charge;
    Particle particle[noOfParticles];
    Particle electron, proton, muon, positron, photon, antiproton;
    void assign(){
        particle[noOfParticles];

        electron = Particle("Electron", 9.10938356*pow(10,-31), -::q, 0.5);
        electron.assignSpeed(findSpeed(electron, momentum));
        particle[0] = electron;

        proton = Particle("Proton", 1.6726219*pow(10,-27), ::q, 0.5);
        proton.assignSpeed(findSpeed(proton, momentum));
        particle[1] = proton;
        
        muon = Particle("Muon", 1.8835*pow(10,-28), -::q, 0.5);
        muon.assignSpeed(findSpeed(muon, momentum));
        particle[2] = muon;
        
        positron = Particle("Positron", 9.10938356*pow(10,-31), ::q, 0.5);
        positron.assignSpeed(findSpeed(positron, momentum));
        particle[3] = positron;
        
        photon = Particle("Photon",0,0,1);
        photon.assignSpeed(findSpeed(photon, momentum));
        particle[4] = photon;
        
        antiproton = Particle("Anti-Proton",1.6726219*pow(10,-27),-::q,0.5);
        antiproton.assignSpeed(findSpeed(antiproton, momentum));
        particle[5] = antiproton;
    }
    Beam(){ 
        assign();
    }
    Beam(double p){
        momentum.updateData("relativistic", p);
        assign();
    }
    Beam(double p, long c){
        momentum.updateData("relativistic", p);
        if(c){
            charge = POSITIVE;
        } else {
            charge = NEGATIVE;
        }
        assign();
    }
    Beam(double p, char a){
        momentum.updateData("relativistic", p);
        if(a == '+'){
            charge = POSITIVE;
        } else if (a == '-') {
            charge = NEGATIVE;
        }
        assign();
    }
    void updateData(char type[], double data){
        if(!strcmp(type, "momentum") || !strcmp(type, "Momentum")){
            momentum.updateData("relativistic", data);
            assign();
        }
        else if(!strcmp(type, "charge")){
            charge = !charge;
            cout<<"Charge = "<<charge;
        }
        else{
            cout<<"No member called "<<type<<endl;
        }
    }
    void display(){
        cout<<"Momentum: "<<momentum.relativistic<<"GeV/c"<<endl;
        for(int i = 0; i < noOfParticles; i++){
            if (charge == POSITIVE && particle[i].charge > 0){
                particle[i].display(); 
            } 
            if (charge == NEGATIVE && particle[i].charge < 0){
                particle[i].display();
            }
        }
    }
    ClassDef(Beam,1);
};

double lorentzRadius(Particle p, Beam b, Magnet m){
    return b.momentum.SI/(p.charge*m.strength);
}

//(1)
double angleOfDeviation(Particle p, Beam b, Magnet m){
    return asin(m.width/lorentzRadius(p,b,m));
}
//(2)
double angleOfDeviation(Particle p, Beam b, Magnet m, double amps){ //bp=bending power of magnet
    return asin((getBP(amps)*p.charge)/b.momentum.SI);
}
//(3)
double angleOfDeviation(double deviation, Magnet m, Detector mm, int rootNumber){
    p = Polynomial(m.width,-deviation,m.width-2*((mm.distance-m.distance)+m.width),deviation);
    std::vector<double> roots = p.FindRealRoots();
    return 2*atan(roots[rootNumber]);
}

// double lateralDeviation(Particle p, Beam b, Magnet m, double amps, Detector mm){
//     double a = angleOfDeviation(p,b,m,amps)/2;
//     return (m.width*pow(tan(a),3)+(m.width-2*((mm.distance-m.distance)+m.width))*tan(a))/(pow(tan(a),2)-1);
// }

double lateralDeviation(Particle p, Beam b, Magnet m, double amps, Detector mm){
    double a = angleOfDeviation(p,b,m,amps);
    return tan(a)*(mm.distance-m.distance);
}
double lateralDeviation(Detector mm){
    if(beam.momentum.relativistic>0){
        double a = angleOfDeviation(beam.proton, beam, mdx27, amps);
        return tan(a)*(mm.distance-mdx27.distance);
    } else {
        cout<<"Error: No beam information.\n";
        return NULL;
    }
}

//(1)
double findCharge(double angle, Beam b, Magnet m){ //original
    return (b.momentum.SI*sin(angle)/(m.strength*m.width));
}
//(2)
double findCharge(double angle, Beam b, double amps){ //using bp with amperage input
    return (b.momentum.SI*sin(angle)/(getBP(amps)));
}
//(3)
double findCharge(double angleX, double angleY, Beam b, double amps, bool a){
    return findCharge(getZAngle(angleX,angleY), b, amps); //calls upon (2), may be redundant
}
//(4)
double findCharge(double angleX, double angleY, Beam b, double amps){ //Aayush derived this one
    return ((sin(angleX)*b.momentum.SI)/(getBP(amps)*cos(angleY)));
}


double getZAngle(double x, double y){
    return acos(sqrt(1-sin(x)*sin(x)-sin(y)*sin(y)));
}

// double getBP(double amps){
//     double bp=0;
//     if(amps==60)
//     {
//         //cout<<"Note: Amperage (60A) changed to bending power (0.13333Tm)\n";
//         bp=0.1333;
//     }
//     else if(amps==120)
//     {
//         //cout<<"Note: Amperage (120A) changed to bending power (0.267Tm)\n";
//         bp=0.267;
//     }
//     else if(amps==180)
//     {
//         //cout<<"Note: Amperage (180A) changed to bending power (0.4Tm)\n";
//         bp=0.4;
//     }
//     else if(amps==240)
//     {
//         //cout<<"Note: Amperage (240A) changed to bending power (0.51Tm)\n";
//         bp=0.51;
//     }
//     if(amps>240){
//         cout<<"Error: bending power can only be <= 0.51\n";
//         cout<<"Please input bending power in Tm, or 60/120/180/240A";
//     }
//     return bp;
// }
double getBP(int amps){  //source: Cristavao
    double bp = 0;
    switch (amps){
        case 10:  bp = 0.022521; break;
        case 20:  bp = 0.045042; break;   
        case 30:  bp = 0.067563; break;
        case 40:  bp = 0.090084; break;
        case 50:  bp = 0.112605; break;
        case 60:  bp = 0.135126; break;
        case 70:  bp = 0.157648; break;
        case 80:  bp = 0.180169; break;
        case 90:  bp = 0.202690; break;
        case 100: bp = 0.225211; break;
        case 110: bp = 0.247732; break;
        case 120: bp = 0.270253; break;
        case 130: bp = 0.292774; break;
        case 140: bp = 0.315295; break;
        case 150: bp = 0.337816; break;
        case 160: bp = 0.360337; break;
        case 170: bp = 0.382534; break;
        case 180: bp = 0.403731; break;
        case 190: bp = 0.423909; break;
        case 200: bp = 0.443067; break;
        case 210: bp = 0.461204; break;
        case 220: bp = 0.478322; break;
        case 230: bp = 0.494420; break;
        case 240: bp = 0.509498; break;
        case 250: bp = 0.523556; break;
        default: 
            cout<<"Please input amps as multiples of 10 only, range: 10 to 250 Amperes\n";
            if(amps > 9){
                if(amps < 260){
                    cout<<"Using bending power for "<<floor(amps/10)*10<<" Amperes\n";
                    return getBP(floor(amps/10)*10);
                }
                else{
                    cout<<"Using bending power for 250 Amperes\n";
                    return getBP(250);
                }
            }
            break;
    }
    return bp;
}
double min(double a, double b, double c, double d){
    if(a<=b && a<=c && a<=d)
        return a;
    else
        if(b<=c && b<=d)
            return b;
        else
            if(c<=d)
                return c;
            else
                return d;
}

double degrees(double radians){
    return radians*180/::pi;
}

double findSpeed(Particle p, Momentum m){
    if(p.mass.rest != 0){
        double numerator = m.SI*::c;
        double denominator = sqrt((p.mass.rest*p.mass.rest*::c*::c)+(m.SI*m.SI));
        double v = numerator/denominator;
    } else {
        double v = ::c;
    }
    return v;
}

double findTime(double speed, double distance){
    return distance/speed;
}

// void autoAssign(){
//  beam=Beam(1,'+');
//  //magnet=Magnet(1.0);
//  magnet=Magnet("MDX27",0.5);
//  mm=Detector("MicroMega",1);
//  cout<<"Assigned 1 GeV/c Beam beam, 0.5T/0.55m Magnet magnet (equiv to MDX27), 1m away Detector mm (MicroMega) \n";
// }


class Help{
    public:
    Help(){}
    void Beam(){
        cout<<"To assign Beam\n";
        cout<<" Expecting: \n";
        cout<<"     Beam() \n";
        cout<<"     Beam(double Momentum(GeV))\n";
        cout<<"     Beam(double Momentum(GeV), POSITIVE *or* NEGATIVE)\n";
        cout<<"     Beam(double Momentum(GeV), '+' *or* '-')\n";
    }
    void Magnet(){
        cout<<"To assign Magnet\n";
        cout<<"     Expecting: \n";
        cout<<"     Magnet(double strength)\n";
    }
    void Particle(){
        cout<<"To assign Particle\n";
        cout<<"     Expecting: \n";
        cout<<"     Particle(char name[], double mass, double charge, double spin)\n";
    }
    void lorentzRadius(){
        cout<<"To execute lorentzRadius\n";
        cout<<" Expecting:\n";
        cout<<" lorentzRadius(Particle p, Beam b, Magnet m)\n";
    }
    void angleOfDeviation(){
        cout<<"To execute angleOfDeviation \n";
        cout<<" Expecting: \n";
        cout<<" angleOfDeviation(Particle p, Beam b, Magnet m) \n";
        cout<<" angleOfDeviation(double deviation, Magnet m, Detector mm, int rootNumber)\n";
    }
    void lateralDeviation(){
        cout<<"To execute lateralDeviation \n";
        cout<<" Expecting:\n";
        cout<<" lateralDeviation(Particle p, Beam b, Magnet m, Detector mm)";
    }
    void classes(){
        cout<<"***Classes***\n\n";
        Beam();
        Particle();
        Magnet();
        cout<<endl;
    }
    void functions(){
        cout<<"\n***Functions***\n\n";
        lorentzRadius();
        angleOfDeviation();
        lateralDeviation();
        cout<<endl;
    }
    void all(){
        cout<<"***Classes***\n\n";
        Beam();
        Particle();
        Magnet();
        cout<<"\n***Functions***\n\n";
        lorentzRadius();
        angleOfDeviation();
        lateralDeviation();
        cout<<endl;
    }
    ClassDef(Help,1);
}help;

// Beam::ClassDef(Beam,1);
// Mass::ClassDef(Mass,1);
// Particle::ClassDef(Particle,1);
// Momentum::ClassDef(Momentum,1);
// Magnet::ClassDef(Magnet,1);

/*
int main(){
    Beam beam(1,'-');
    beam.display();
    cout<<beam.electron.mass.relativistic;
    return 0;
}*/

    //crypticontics.h
    //13th January 2019

    Beam beam(6.25, '+');
    Magnet mdx27("MDX27CO", 1.0);
    const Detector mm1("MicroMega", 8.022);
    const Detector mm2("MicroMega", 8.859);
    const Detector mm3("MicroMega", 8.777);
    const Detector mm4("MicroMega", 8.104);
    const Detector dwc0("DWC", -0.028);
    const Detector dwc1("DWC", 4.358);
    const Detector dwc2("DWC", 5.112);
    const Detector dwc3("DWC", 7.159);

    void defaultSetup(){
        beam=Beam(6.25,'+');
        mdx27=Magnet("MDX27CO",1,0);
        cout<<"Assigned: 6.25 GeV/c positive Beam beam"<<endl;
        cout<<"1T Magnet mdx27"<<endl;
        cout<<"Detectors mm1,mm2,mm3,mm4,dwc0,dwc1,dwc2,dwc3" <<endl;
        cout<<"All at predefined distances as per absolute Z value in Jorgen's document in meters"<<endl;
    }

    double findAngle(double d1, double d2, double d3){
        int m = 3;
        double y[3]={d1,d2,d3};
        double x[3]={dwc3.distance*1000, mm1.distance*1000, mm3.distance*1000};
        double theta[2]={0, 0};

        double sumY=0.0;
        double sumX=0.0;
        double sumXY=0.0;
        double sumXs=0.0;
  
        for (int i = 0; i<m; i++) {
            sumY += y[i];
            sumX += x[i];
            sumXs += (x[i] * x[i]);
            sumXY += (x[i] * y[i]);
        }

        double denominator = ((sumXs) * m) - (sumX * sumX);

        theta[0] = ((sumXs * sumY) - (sumX * sumXY)) / denominator;
        theta[1] = ((m * sumXY) - (sumX * sumY)) / denominator;
        //cout<<atan(theta[1])<<endl;
        return (atan(theta[1]));
    }

    bool checkChargeAccuracy(double angle, Beam b, Magnet m){
        double check = fabs(1-findCharge(angle, b, m)/::q);
        cout<<check<<"\n";
        if(check < 0.1){
            return true;
        }
        else{
            return false;
        }
    }

    char* getRunFileNumber(){
        gROOT->Reset();
        cout<<"Input file name\n";
        char f[15];
        cin>>(f);
        return f;
    }

    double calccharge(int p, double BP, double valueX, double initY){ //Aayush likes this one but I like the next one
        return ((sin(valueX)*p*5.36)/(BP*cos(initY)));
    }
    double calccharge(double totalX, double initialY, Beam b, double amps){
        return ((sin(totalX)*b.momentum.SI)/(getBP(amps)*cos(initialY)));
    }

    void getTimeDifference(){
        cout<<"Boop boop";
    }

//runDatabase.h
//25-02-2019


const int maxRuns = 150; //perhaps not needed if runSelection becomes a vector and other functions are likewise changed
//total 150 runs, 9 properties to check
double runList[150][7];
string runPOIType[150][2];

int selectedRuns = 0;

bool initiated = false;

#define ALL -1

//properties defined below
#define RUNNUMBER 0
#define AMPERES 1
#define MOMENTUM 2
#define RECORDED 3
#define GOOD 4
#define MUONFILTER 5
#define ISOK 6
#define TYPE 0
#define POI 1

int runSelection[maxRuns]; //would be much better if it is a vector and not an array

void findRuns(double momentum = ALL, double amps = ALL, int mf = ALL, string type=""){
    //writes runs to runSummary (after zeroing all indices) based on user specified run properties.
    //properties are optional, e.g., to find all 120A runs, use findRuns(ALL, 120);
    //User can search for only positive results by writing e.g. findRuns(5000, '+');
    if(!initiated){
        initiateDatabase();
    }
    clearSelection();
    int count=0;
    for(int i=0;i<maxRuns;i++){
        if(checkMomentum(i, momentum, (momentum==ALL))){
            if(checkAmps(i, amps, (amps==ALL))){
                if(checkMuonFilter(i, mf, (mf==ALL))){
                    if(checkType(i,type, (type==""))){
                        runSelection[count] = runList[i][RUNNUMBER];
                        //cout<<int(runList[i][RUNNUMBER])<<endl;
                        count++;
                    }
                }
            }
        }
    }
    cout<<"Number of Runs: "<<count<<endl;
    if(count==0){
        cout<<"No runs found with these conditions in database\n";
    }
    selectedRuns = count;
    //cout<<"Call run numbers from runSelection[]"<<endl;
}
int appendRunSelection(double momentum = ALL, double amps = ALL, int mf = ALL, string type=""){
    //appends run(s) to the end of runSelection by searching based on run properties
    //quite messy and inefficient
    int originalSelectedRuns = selectedRuns;
    if(originalSelectedRuns!=0){    
        int originalRunSelection[maxRuns];
        int newRunSelection[maxRuns];
        int newSelectedRuns=0;
        copyArray(runSelection,originalRunSelection,selectedRuns);
        findRuns(momentum,amps,mf,type);
        newSelectedRuns=selectedRuns;
        copyArray(runSelection, newRunSelection, selectedRuns);
        clearSelection();
        selectedRuns=originalSelectedRuns;
        copyArray(originalRunSelection, runSelection, originalSelectedRuns);
        for(int i=0;i<newSelectedRuns;i++){
            appendRunSelection(newRunSelection[i],false);
        }
        return 0;
    } else {
        findRuns(momentum, amps, mf, type);
        return 0;
    }
}
int appendRunSelection(int rn, bool verbose = true){
    //appends single run to the end of runSelection based on run number
    if(runExists(rn)){
        for(int i=0;i<selectedRuns;i++){
            if(runSelection[i]==rn){
                if(verbose){
                    cout<<"Run "<<rn<<" already in runSelection\n";
                    cout<<"Index at: "<<i<<endl;
                }
                return 0;
            }
        }
        if(selectedRuns<maxRuns){
            runSelection[selectedRuns]=rn;
            selectedRuns++;
            if(verbose){
                cout<<"Run "<<rn<<" added to runSelection\n";
                cout<<"Number of runs (selectedRuns) = "<<selectedRuns<<endl;
            }
            return 0;
        } else {
            cout<<"Max number of selectable runs exceeded\n";
            return -1;
        }
    } else {
        if(verbose){
            cout<<"Run "<<rn<<" does not exist\n";
        }
        return -1;
    }
}
void findAndLoadRunWithMaxEntries(double momentum = ALL, double amps = ALL, int mf = ALL, bool micromega = true, string type="", double fast=true){
    findRuns(momentum, amps, mf, type);
    if(selectedRuns!=0){
        int toLoad=getMaxEntriesIndexInSelection(fast, true, micromega);
        loadAllDetectors(runSelection[toLoad],false);
        cout<<"Loaded Run "<<runSelection[toLoad]<<endl;
        getRunDetails(runSelection[toLoad], true);
    }
}
void getRunDetails(bool verbose = false){
    int i = 0;
    while(runSelection[i]!=0){
        cout<<"\nRun Index:     "<<i;
        cout<<"\n\tRun Number:  "<<int(runSelection[i]);
        cout<<"\n\tMomentum:    "<<getMomentum(int(runSelection[i]));
        cout<<"\n\tAmperes:     "<<getAmps(int(runSelection[i]));
        cout<<"\n\tMuon Filter: "<<getMuonFilter(int(runSelection[i]));
        if(verbose){
            cout<<"\n\tRecorded:    "<<getRecorded(int(runSelection[i]));
            cout<<"\n\tGood:        "<<getGood(int(runSelection[i]));
            cout<<"\n\tisOk         "<<getisOK(int(runSelection[i]));
            cout<<"\n\tType:        "<<getType(int(runSelection[i]));
            cout<<"\n\tPOI:         "<<getPOI(int(runSelection[i]));
        }
        i++;
    }
    if(runSelection[0]==0){
        cout<<"No runs selected \n";
    }
    cout<<endl<<endl;
}
void getRunDetails(int rn, bool verbose = false){
    for(int i=0;i<maxRuns;i++){
        if(runList[i][0]==rn){
            cout<<"\n\tRun Number:  "<<int(rn);
            cout<<"\n\tMomentum:    "<<getMomentum(rn);
            cout<<"\n\tAmperes:     "<<getAmps(rn);
            cout<<"\n\tMuon Filter: "<<getMuonFilter(rn);
            if(verbose){
                cout<<"\n\tRecorded:    "<<getRecorded(rn);
                cout<<"\n\tGood:        "<<getGood(rn);
                cout<<"\n\tisOk         "<<getisOK(rn);
                cout<<"\n\tType:        "<<getType(rn);
                cout<<"\n\tPOI:         "<<getPOI(rn);
            }
            break;
        }
    }
    cout<<endl<<endl;
}
void getLoadedRunDetails(){
    if(runNumberString.str()!=""){
        getRunDetails(atoi(runNumberString.str().c_str()),true);
    } else {
        cout<<"No run loaded\n";
    }
}
bool runExists(int rn){
    for(int j=0;j<maxRuns;j++){
        if(runList[j][0]==rn){
            return true;
        }
    }
    return false;
}
double getAmps(int rn){
    double p = 0;
    for(int i=0;i<maxRuns;i++){
        if(runList[i][0]==rn){
            p = runList[i][AMPERES];
            return p;
        }
    }
    return p;
}
double getMomentum(int rn){
    double p = 0;
    for(int i=0;i<maxRuns;i++){
        if(runList[i][0]==rn){
            p = runList[i][MOMENTUM];
            return p;
        }
    }
    return p;
}
double getRecorded(int rn){
    double p = 0;
    for(int i=0;i<maxRuns;i++){
        if(runList[i][0]==rn){
            p = runList[i][RECORDED];
            return p;
        }
    }
    return p;
}
double getGood(int rn){
    double p = 0;
    for(int i=0;i<maxRuns;i++){
        if(runList[i][0]==rn){
            p = runList[i][GOOD];
            return p;
        }
    }
    return p;
}
double getMuonFilter(int rn){
    double p = 0;
    for(int i=0;i<maxRuns;i++){
        if(runList[i][0]==rn){
            p = runList[i][MUONFILTER];
            return p;
        }
    }
    return p;
}
double getisOK(int rn){
    double p = 0;
    for(int i=0;i<maxRuns;i++){
        if(runList[i][0]==rn){
            p = runList[i][ISOK];
            return p;
        }
    }
    return p;
}
string getType(int rn){
    string p = "";
    for(int i=0;i<maxRuns;i++){
        if(runList[i][0]==rn){
            p = runPOIType[i][TYPE];
            return p;
        }
    }
    return p;
}
string getPOI(int rn){
    string p = "";
    for(int i=0;i<maxRuns;i++){
        if(runList[i][0]==rn){
            p = runPOIType[i][POI];
            return p;
        }
    }
    return p;
}
bool checkMomentum(int rn, double n, bool bypass=false){
    if(n==POSITIVE){
        if(runList[rn][MOMENTUM]>0){
            return true;
        }
    }
    else if(n==NEGATIVE){
        if(runList[rn][MOMENTUM]<0){
            return true;
        }
    }
    else if(runList[rn][MOMENTUM]==n || bypass){
        return true;
    }
    else{
        return false;
    }
}
bool checkAmps(int rn, double n, bool bypass=false){
    if(n=='+'){
        if(runList[rn][AMPERES] == 0){
            return false;
        } else {
            bypass=true;
        }
    }
    if(runList[rn][AMPERES] == n || bypass){
        return true;
    } 
    else{
        return false;
    }
}
bool checkMuonFilter(int rn, bool n = true, bool bypass=false){
    if(runList[rn][MUONFILTER]==n || bypass){
        return true;
    }
    else{
        return false;
    }
}
bool checkType(int rn, string t, bool bypass=false){
    if(runPOIType[rn][TYPE]==t || bypass){
        return true;
    }
    else{
        return false;
    }
}
int getMaxEntriesIndexInSelection(bool fast=true, bool verbose=false, bool micromega){
    int max=0, maxIndex;
    int maxTemp;
    if(selectedRuns!=0){
        for(int i=0;i<selectedRuns;i++){
            if(getisOK(runSelection[i])){
                if(fast){
                    maxTemp = getGood(runSelection[i]);
                } else {
                    loadAllDetectors(runSelection[i],false);
                    maxTemp = t->GetEntries();
                }
                if(micromega){
                    loadAllDetectors(runSelection[i],false);
                }
                if(!micromega || t->GetListOfBranches()->FindObject("NBL4Smm1_stripQ")){
                    if(maxTemp>max){
                        max=maxTemp;
                        maxIndex=i;
                    }
                }
            }
        }
        if(verbose){
            cout<<"\nRun Number:            "<<runSelection[maxIndex]<<endl;
            cout<<"Entries:               "<<max<<endl;
            cout<<"Index in runSelection: "<<maxIndex<<endl<<endl;
        }
        return maxIndex;
    }else{
        if(verbose){
            cout<<"No runs to choose from"<<max<<endl;
        }
        return -1;
    }
}
void clearSelection(){
    for(int i=0;i<maxRuns;i++){
        runSelection[i]=0;
    }
    selectedRuns=0;
}
void clearSelection(double arr[]){
    for(int i=0;i<selectedRuns;i++){
        arr[i]=0;
    }
}
void copyArray(int src[], int dest[], int size){
    for(int i=0;i<size;i++){
        dest[i]=src[i];
    }
}
void addRunData(int run, int rn, int amp, double p, string t, string poi, int r, int g, bool mf, bool isok){
    runList[run][0] = rn;     //Run Number
    runList[run][1] = amp;    //amperes
    runList[run][2] = p;      //momentum
    runList[run][3] = r;      //recorded
    runList[run][4] = g;      //good
    runList[run][5] = mf;     //muon filter
    runList[run][6] = isok;   //isOK
    runPOIType[run][0] = t.c_str();   //Type
    runPOIType[run][1] = poi.c_str(); //POI
    //cout<<"Added run "<<run<<endl;
}
void initiateDatabase(){
    addRunData (0,1537523826, 0, 0, "calibration", "DWC Calibration", 20218, 20218, false, false); 
    addRunData (1,1537524073, 0, 0, "calibration", "DWC Calibration", 20135, 20135, false, false); 
    addRunData (2,1537524312, 0, 0, "calibration", "DWC Calibration", 20166, 20166, false, false); 
    addRunData (3,1537524775, 0, 0, "calibration", "DWC Calibration", 20223, 20223, false, true); 
    addRunData (4,1537524993, 0, 0, "calibration", "DWC Calibration", 20220, 20220, false, true); 
    addRunData (5,1537525209, 0, 0, "calibration", "DWC Calibration", 20176, 20176, false, true); 
    addRunData (6,1537525452, 0, 0, "calibration", "DWC Calibration", 20206, 20206, false, true); 
    addRunData (7,1537525665, 0, 0, "calibration", "DWC Calibration", 20196, 20196, false, true); 
    addRunData (8,1537525880, 0, 0, "calibration", "DWC Calibration", 20339, 20339, false, true); 
    addRunData (9,1537526116, 0, 0, "calibration", "DWC Calibration", 20191, 20191, false, true); 
    addRunData (10,1537526329, 0, 0, "calibration", "DWC Calibration", 20156, 20156, false, true); 
    addRunData (11,1537526544, 0, 0, "calibration", "DWC Calibration", 20157, 20157, false, true); 
    addRunData (12,1537605192, 0, 10000, "calibration", "TOF Offset", 7687, 7476, false, true); 
    addRunData (13,1537619367, 0, 10000, "calibration", "First Beam", 20393, 20393, false, false); 
    addRunData (14,1537622088, 0, 10000, "calibration", "TOF Offset", 17679, 17165, false, true); 
    addRunData (15,1537645668, 0, 10000, "calibration", "Scuba Calibration", 246857, 246856, false, true); 
    addRunData (16,1537691286, 0, 10000, "deflection", "0:False", 33833, 33833, false, false); 
    addRunData (17,1537694541, 0, 10000, "deflection", "0:False", 32741, 32741, false, true); 
    addRunData (18,1537696203, 0, 10000, "deflection", "0:False", 9764, 9764, false, true); 
    addRunData (19,1537696741, 0, 10000, "deflection", "0:False", 3244, 3244, false, true); 
    addRunData (20,1537696963, 0, 10000, "deflection", "0:False", 2731, 2731, false, true); 
    addRunData (21,1537697126, 0, 10000, "deflection", "0:False", 22374, 22374, false, true); 
    addRunData (22,1537698296, 0, 10000, "deflection", "0:False", 10388, 10388, false, true); 
    addRunData (23,1537700167, 0, 10000, "deflection", "0:False", 18689, 18689, false, true); 
    addRunData (24,1537703559, 0, 10000, "deflection", "0:False", 53125, 53125, false, true); 
    addRunData (25,1537711702, 0, 10000, "deflection", "0:False", 7737, 7737, false, true); 
    addRunData (26,1537712176, 0, 10000, "deflection", "0:False", 6588, 6588, false, true); 
    addRunData (27,1537712564, 0, 10000, "deflection", "0:False", 4846, 4846, false, true); 
    addRunData (28,1537712976, 0, 10000, "deflection", "0:False", 6588, 6588, false, true); 
    addRunData (29,1537715214, 0, 10000, "deflection", "0:False", 44768, 44768, false, true); 
    addRunData (30,1537719413, 0, 10000, "deflection", "0:False", 8887, 8887, false, true); 
    addRunData (31,1537720422, 0, 10000, "deflection", "0:False", 6129, 6129, false, true); 
    addRunData (32,1537720918, 0, 10000, "deflection", "0:False", 3310, 3310, false, true); 
    addRunData (33,1537721187, 0, 10000, "deflection", "0:False", 34855, 34855, false, true); 
    addRunData (34,1537723600, 0, 10000, "deflection", "0:False", 4476, 4476, false, true); 
    addRunData (35,1537723950, 0, 10000, "deflection", "0:False", 2911, 2911, false, true); 
    addRunData (36,1537724219, 0, 10000, "deflection", "0:False", 3528, 3528, false, true); 
    addRunData (37,1537781816, 0, 600, "deflection", "0:False", 9619, 9619, false, true); 
    addRunData (38,1537783395, 0, 10000, "deflection", "0:False", 15024, 15015, false, true); 
    addRunData (39,1537784652, 0, 10000, "deflection", "0:False", 13789, 13691, false, true); 
    addRunData (40,1537785478, 0, 10000, "deflection", "0:False", 38, 38, false, true); 
    addRunData (41,1537786038, 0, 10000, "deflection", "0:False", 546, 546, false, true); 
    addRunData (42,1537789573, 0, 10000, "deflection", "0:False", 175, 175, false, true); 
    addRunData (43,1537792175, 0, 10000, "deflection", "0:False", 563, 563, false, true); 
    addRunData (44,1537796537, 0, 10000, "deflection", "0:False", 1050, 1050, false, true); 
    addRunData (45,1537802100, 0, 10000, "deflection", "0:False", 7, 7, false, false); 
    addRunData (46,1537802128, 240, 10000, "deflection", "240:False", 497, 497, false, true); 
    addRunData (47,1537803772, 120, 10000, "deflection", "120:False", 501, 501, false, true); 
    addRunData (48,1537814423, 0, -10000, "deflection", "0:False", 12813, 12813, false, true); 
    addRunData (49,1537862811, 0, -10000, "deflection", "0:False", 79, 79, false, true); 
    addRunData (50,1537867107, 240, 5000, "deflection", "240:False", 1012, 1012, false, true); 
    addRunData (51,1537868335, 120, 5000, "deflection", "120:False", 1001, 1001, false, true); 
    addRunData (52,1537869886, 0, 5000, "deflection", "0:False", 996, 996, false, true); 
    addRunData (53,1537895191, 0, 0, "calibration", "DWC Calibration", 20330, 20330, false, false); 
    addRunData (54,1537895268, 0, 0, "calibration", "DWC Calibration", 20401, 20401, false, false); 
    addRunData (55,1537895345, 0, 0, "calibration", "DWC Calibration", 20551, 20551, false, false); 
    addRunData (56,1537895618, 0, 0, "calibration", "DWC Calibration", 150, 150, false, false); 
    addRunData (57,1537895764, 0, 0, "calibration", "DWC Calibration", 20546, 20546, false, true); 
    addRunData (58,1537895851, 0, 0, "calibration", "DWC Calibration", 20267, 20267, false, true); 
    addRunData (59,1537895941, 0, 0, "calibration", "DWC Calibration", 20232, 20232, false, true); 
    addRunData (60,1537896105, 0, 0, "calibration", "DWC Calibration", 20165, 20165, false, true); 
    addRunData (61,1537896220, 0, 0, "calibration", "DWC Calibration", 20203, 20203, false, true); 
    addRunData (62,1537896312, 0, 0, "calibration", "DWC Calibration", 23822, 23822, false, true); 
    addRunData (63,1537896423, 0, 0, "calibration", "DWC Calibration", 20929, 20929, false, true); 
    addRunData (64,1537896503, 0, 0, "calibration", "DWC Calibration", 22060, 22060, false, true); 
    addRunData (65,1537896576, 0, 0, "calibration", "DWC Calibration", 20995, 20995, false, true); 
    addRunData (66,1537898557, 0, -10000, "calibration", "Profile", 9432, 9432, false, true); 
    addRunData (67,1537900489, 12, 600, "deflection", "12:False", 26050, 26020, false, true); 
    addRunData (68,1537903575, 60, 10000, "deflection", "60:False", 1040, 1040, false, true); 
    addRunData (69,1537904292, 180, 10000, "deflection", "180:False", 1061, 1061, false, true); 
    addRunData (70,1537906219, 240, 10000, "deflection", "240:False", 1033, 1033, false, true); 
    addRunData (71,1537909725, 0, 5000, "deflection", "0:False", 1024, 1024, false, true); 
    addRunData (72,1537910138, 60, 5000, "deflection", "60:False", 1085, 1085, false, true); 
    addRunData (73,1537910395, 120, 5000, "deflection", "120:False", 1122, 1122, false, true); 
    addRunData (74,1537910721, 120, 5000, "deflection", "120:False", 0, 0, false, false); 
    addRunData (75,1537910732, 180, 5000, "deflection", "180:False", 1243, 1243, false, true); 
    addRunData (76,1537911018, 240, 5000, "deflection", "240:False", 1242, 1242, false, true); 
    addRunData (77,1537913470, 240, -10000, "deflection", "240:False", 27296, 27296, false, true); 
    addRunData (78,1537942404, 120, -10000, "deflection", "120:False", 5427, 5427, false, true); 
    addRunData (79,1538000916, 60, -10000, "deflection", "60:False", 186, 186, false, true); 
    addRunData (80,1538001499, 60, -10000, "deflection", "60:False", 4016, 4016, false, true); 
    addRunData (81,1538003771, 180, -10000, "deflection", "180:False", 2362, 2362, false, true); 
    addRunData (82,1538006510, 240, -10000, "deflection", "240:False", 2560, 2560, false, true); 
    addRunData (83,1538008044, 0, -10000, "deflection", "0:False", 29231, 29231, false, true); 
    addRunData (84,1538031580, 0, 5000, "deflection", "0:True", 2259, 2054, true, true); 
    addRunData (85,1538038031, 120, 5000, "deflection", "120:True", 1714, 1157, true, true); 
    addRunData (86,1538042143, 240, 5000, "deflection", "240:True", 1763, 1010, true, true); 
    addRunData (87,1538046170, 60, 5000, "deflection", "60:True", 1902, 1080, true, true); 
    addRunData (88,1538049472, 180, 5000, "deflection", "180:True", 1927, 998, true, true); 
    addRunData (89,1538052756, 240, -5000, "deflection", "240:True", 600, 360, true, true); 
    addRunData (90,1538055641, 0, -5000, "deflection", "0:True", 605, 305, true, true); 
    addRunData (91,1538058151, 240, 3500, "deflection", "240:True", 2003, 1192, true, true); 
    addRunData (92,1538063096, 240, 10000, "deflection", "240:True", 1410, 1393, true, true); 
    addRunData (93,1538064369, 120, 10000, "deflection", "120:True", 1522, 1287, true, true); 
    addRunData (94,1538066109, 60, 10000, "deflection", "60:True", 1563, 964, true, true); 
    addRunData (95,1538067822, 180, 10000, "deflection", "180:True", 1514, 964, true, true); 
    addRunData (96,1538068869, 0, 10000, "deflection", "0:True", 1694, 981, true, true); 
    addRunData (97,1538069833, 0, -10000, "deflection", "0:True", 1825, 1013, true, true); 
    addRunData (98,1538074506, 120, -10000, "deflection", "120:True", 796, 771, true, true); 
    addRunData (99,1538077069, 180, -10000, "deflection", "180:True", 769, 724, true, true); 
    addRunData (100,1538079107, 60, -10000, "deflection", "60:True", 812, 784, true, true); 
    addRunData (101,1538081315, 240, -10000, "deflection", "240:True", 766, 766, true, true); 
    addRunData (102,1538083650, 120, -5000, "deflection", "120:True", 912, 798, true, true); 
    addRunData (103,1538085941, 180, -5000, "deflection", "180:True", 911, 530, true, true); 
    addRunData (104,1538088606, 60, -5000, "deflection", "60:True", 935, 935, true, true); 
    addRunData (105,1538091404, 0, -5000, "deflection", "0:True", 5419, 5383, true, true); 
    addRunData (106,1538125518, 0, 0, "calibration", "DWC Calibration", 40896, 40896, false, true); 
    addRunData (107,1538125664, 0, 0, "calibration", "DWC Calibration", 41255, 41255, false, true); 
    addRunData (108,1538125793, 0, 0, "calibration", "DWC Calibration", 69084, 69084, false, true); 
    addRunData (109,1538126282, 0, 0, "calibration", "DWC Calibration", 10, 10, false, false); 
    addRunData (110,1538126300, 0, 0, "calibration", "DWC Calibration", 65417, 65417, false, true); 
    addRunData (111,1538126534, 0, 0, "calibration", "DWC Calibration", 42427, 42427, false, true); 
    addRunData (112,1538126651, 0, 0, "calibration", "DWC Calibration", 42428, 42428, false, true); 
    addRunData (113,1538126801, 0, 0, "calibration", "DWC Calibration", 43096, 43096, false, true); 
    addRunData (114,1538126926, 0, 0, "calibration", "DWC Calibration", 2516, 2516, false, false); 
    addRunData (115,1538126941, 0, 0, "calibration", "DWC Calibration", 42302, 42302, false, true); 
    addRunData (116,1538127056, 0, 0, "calibration", "DWC Calibration", 42430, 42430, false, true); 
    addRunData (117,1538127195, 0, 0, "calibration", "DWC Calibration", 42996, 42996, false, true); 
    addRunData (118,1538127316, 0, 0, "calibration", "DWC Calibration", 44869, 44869, false, false); 
    addRunData (119,1538127495, 0, 0, "calibration", "DWC Calibration", 41508, 41508, false, true); 
    addRunData (120,1538127617, 0, 0, "calibration", "DWC Calibration", 42203, 42203, false, true); 
    addRunData (121,1538170034, 0, -5000, "deflection", "0:False", 96210, 96210, false, true); 
    addRunData (122,1538243383, 0, 10000, "calibration", "Scuba Calibration", 1474, 1473, false, true); 
    addRunData (123,1538267157, 240, -5000, "deflection", "240:False", 45731, 45731, false, true); 
    addRunData (124,1538288884, 180, -5000, "deflection", "180:False", 9543, 9543, false, true); 
    addRunData (125,1538293096, 120, -5000, "deflection", "120:False", 3971, 3940, false, true); 
    addRunData (126,1538294874, 60, -5000, "deflection", "60:False", 4090, 4090, false, true); 
    addRunData (127,1538296444, 60, 10000, "deflection", "60:False", 4047, 4047, false, true); 
    addRunData (128,1538297136, 120, 10000, "deflection", "120:False", 5888, 5888, false, true); 
    addRunData (129,1538298064, 0, 10000, "deflection", "0:False", 5020, 5020, false, true); 
    addRunData (130,1538299585, 180, 10000, "deflection", "180:False", 5132, 5132, false, true); 
    addRunData (131,1538300360, 240, 10000, "deflection", "240:False", 5224, 5224, false, true); 
    addRunData (132,1538301037, 240, 5000, "deflection", "240:False", 5186, 5186, false, true); 
    addRunData (133,1538301703, 180, 5000, "deflection", "180:False", 4248, 4248, false, true); 
    addRunData (134,1538302342, 120, 5000, "deflection", "120:False", 4253, 4253, false, true); 
    addRunData (135,1538302979, 60, 5000, "deflection", "60:False", 4029, 4029, false, true); 
    addRunData (136,1538303555, 0, 5000, "deflection", "0:False", 4143, 4143, false, true); 
    addRunData (137,1538304212, 240, 7500, "deflection", "240:False", 4646, 4646, false, true); 
    addRunData (138,1538304657, 240, 3500, "deflection", "240:False", 3333, 3333, false, true); 
    addRunData (139,1538305082, 240, 6250, "deflection", "240:False", 4357, 4357, false, true); 
    addRunData (140,1538305461, 240, 8750, "deflection", "240:False", 5035, 5035, false, true); 
    addRunData (141,1538305980, 240, -8750, "deflection", "240:False", 5043, 5043, false, true); 
    addRunData (142,1538307208, 240, -7500, "deflection", "240:False", 5060, 5060, false, true); 
    addRunData (143,1538308094, 240, -6250, "deflection", "240:False", 5181, 5181, false, true); 
    addRunData (144,1538308890, 240, -3500, "deflection", "240:False", 5095, 5095, false, true); 
    addRunData (145,1538330421, 0, 0, "calibration", "QDC Pedestals", 2801, 2801, false, true); 
    addRunData (146,1538330741, 0, 10000, "calibration", "QDC Pedestals", 14218, 14218, false, true); 
    addRunData (147,1538332209, 0, 10000, "calibration", "TOF Offset", 0, 0, false, false); 
    addRunData (148,1538333479, 0, 10000, "calibration", "TOF Offset", 12989, 12931, false, true); 
    addRunData (149,1538335566, 240, 10000, "deflection", "240:False", 3307, 3307, false, true); 
    initiated=true;
    cout<<"Run Database Initiated\n";
}

// int appendRunSelection(double momentum = ALL, double amps = ALL, int mf = ALL, string type=""){
//  int originalSelectedRuns = selectedRuns;
//  if(originalSelectedRuns!=0){    
//      int originalRunSelection[maxRuns];
//      copyArray(runSelection,originalRunSelection,selectedRuns);
//      findRuns(momentum,amps,mf,type);
//      for(int i=0;i<selectedRuns;i++){
//              runSelection[i+originalSelectedRuns]=runSelection[i];
//      }
//      if(selectedRuns==0){
//          cout<<"No additionals runs found\n";
//      }
//      copyArray(originalRunSelection,runSelection,originalSelectedRuns);
//      selectedRuns+=originalSelectedRuns;
//      return 0;
//  } else {
//      findRuns(momentum,amps,mf,type);
//      return 0;
//  }
// }


    //loadAllDetectors.h
  //21-02-2019


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

//getAccVectors.h
//22-02-2019
std::vector<double> acc[3];
std::vector<double> accT[3];

const int x=0, y=1, z=2;

const double flipX = 1;
const double flipY = 1;
const double flipZ = 1;

void getAccVectors(){
    //EVERYTHING IS IN METERS
    acc[x].clear();
    acc[y].clear();
    acc[z].clear();

    accT[x].clear();
    accT[y].clear();
    accT[z].clear();

    double dwc1pos[3]={0,0,0};
    double dwc2pos[3]={0,0,0};
    double dwc3pos[3]={0,0,0};
    double mm1pos[3]={0,0,0};
    double mm2pos[3]={0,0,0};

    double initial[3]={0,0,0};
    double final[3]={0,0,0};
    double angle[3]={0,0,0};
    double tempAvg=0;
    for(int i = 0; i < entries; i++){
        t->GetEntry(i);
        if (detectedByAll()){ 
            getHitPos(dwc1pos, dwc2pos, dwc3pos, mm1pos, mm2pos);  

            //Vector components
            initial[x] = (flipX) * (dwc2pos[x]-dwc1pos[x]);
            initial[y] = (flipY) * (dwc2pos[y]-dwc1pos[y]);
            initial[z] = (flipZ) * (dwc2pos[z]-dwc1pos[z]);
            
            final[x] = (flipX) * (mm2pos[x]-mm1pos[x]);
            final[y] = (flipY) * (mm2pos[y]-mm1pos[y]);
            final[z] = (flipZ) * (mm2pos[z]-mm1pos[z]);

            //getting angle values
            angle[x] = (atan(final[x]/final[z]) - atan(initial[x]/initial[z]));
            angle[y] = (atan(final[y]/final[z]) - atan(initial[y]/initial[z]));
            angle[z] = (atan(final[z]/final[z]) - atan(initial[z]/initial[z]));

            //Make unit vectors
            makeUnitVector(initial);
            makeUnitVector(final);
            cout<<initial[z]<<"\t";
            cout<<final[z]<<endl;
            pushAccelerations(acc, final, initial, angle);
            pushTheoreticalAccelerations(accT, initial);

        }
    }
    cout<<"Experimental: "<<endl;
        displayAverage(acc[x], "x");
        displayAverage(acc[y], "y");
        displayAverage(acc[z], "z");
    cout<<"\nTheoretical:"<<endl;
        displayAverage(accT[x], "x");
        displayAverage(accT[y], "y");
        displayAverage(accT[z], "z");
}

void makeUnitVector(double a[]){
    double d = sqrt(a[x]*a[x]+a[y]*a[y]+a[z]*a[z]);
    a[x] /= d;
    a[y] /= d;
    a[z] /= d;
}

double getAcceleration(double vectorFinal, double vectorInitial, double angle){
     return (tofSpeed()*(vectorFinal-vectorInitial)/(mdx27.width/(tofSpeed()*cos(angle/2))));
}
void pushAccelerations(std::vector<double> a[], double f[], double i[], double angle[]){
    a[x].push_back(getAcceleration(f[x], i[x], angle[x]));
    a[y].push_back(getAcceleration(f[y], i[y], angle[y]));
    a[z].push_back(getAcceleration(f[z], i[z], angle[z]));
}
void pushTheoreticalAccelerations(std::vector<double> a[], double i[]){
    //double fieldStrength = -(flipY) * getBP(amps) * mdx27.width;
    double fieldStrength = -(flipY) * (amps/240);
    double speed = (flipZ) * tofSpeed();
    double tempq = ::q*(beam.momentum.relativistic>0?1:-1);
    double m = beam.muon.mass.relativistic;
    //double m = ((beam.momentum.SI)/speed);

    a[x].push_back(tempq*speed*(-i[z])*fieldStrength/m);
    a[y].push_back(0);
    a[z].push_back(tempq*speed*(i[x])*fieldStrength/m);
} 
void displayAverage(std::vector<double> d, string xstr){
    double total=0;
    for(int i=0; i<d.size(); i++){
        total+=d.at(i);
    }
    cout<<"Average "<<xstr<<": "<<total/d.size()<<endl;
}

  //trackerScatter.C
  //execute loadAllDetectors.C before running this
  //06-03-2019

void DWCScatter(char n[], bool newCanvas=true){
    double dwcXPos, dwcYPos;
    bool exit = false;
    TH2D* g = new TH2D(name.c_str(),"",100,-100,100,100,-100,100);
    //int validPoints = 0;
    for(int i = 0; i < entries; i++){
        t->GetEntry(i);
        if(strcmp(n,"1")==0){
            g->SetTitle("DWC1 Scatter Plot");
            if (ndwc1L == 1 && ndwc1R == 1 && ndwc1U == 1 && ndwc1D == 1){
                dwcXPos = (dwc1XSlope * (int(dwc1L[0] - dwc1R[0]) + dwc1XOffset) * timeFactor)+dwc1XDisplacement;   
                dwcYPos = (dwc1YSlope * (int(dwc1U[0] - dwc1D[0]) + dwc1YOffset) * timeFactor)+dwc1YDisplacement;
            }   
        }
        else if(strcmp(n,"2")==0){
            g->SetTitle("DWC2 Scatter Plot");
            if (ndwc2L == 1 && ndwc2R == 1 && ndwc2U == 1 && ndwc2D == 1){
                dwcXPos = (dwc2XSlope * (int(dwc2L[0] - dwc2R[0]) + dwc2XOffset) * timeFactor)+dwc2XDisplacement;
                dwcYPos = (dwc2YSlope * (int(dwc2U[0] - dwc2D[0]) + dwc2YOffset) * timeFactor)+dwc2YDisplacement;
            }   
        }
        else if(strcmp(n,"3")==0){
            g->SetTitle("DWC3 Scatter Plot");
            if (ndwc3L == 1 && ndwc3R == 1 && ndwc3U == 1 && ndwc3D == 1){
                dwcXPos = (dwc3XSlope * (int(dwc3L[0] - dwc3R[0]) + dwc3XOffset) * timeFactor)+dwc3XDisplacement;   
                dwcYPos = (dwc3YSlope * (int(dwc3U[0] - dwc3D[0]) + dwc3YOffset) * timeFactor)+dwc3YDisplacement;
            }
        } else {
            cout<<"DWC number not recognized. Only DWC 1, 2 and 3 availible.\n";
            exit = true;
            break;
        }
        g->Fill(dwcXPos, dwcYPos);
        //validPoints++;
    }
    if(!exit){
        g->SetDirectory(0);
        if(newCanvas){
            dwcScatter=new TCanvas("dwcScatter","dwcScatter",400,400);
            dwcScatter->cd();
        }
        g->Draw("colz");
        gStyle->SetOptStat("nem");
        //redundantCanvas();
    }
}

void MMScatter(char n[], bool newCanvas=true){
    bool exit = false;
  if(strcmp(n,"first")==0 || strcmp(n,"1")==0){
    TH2D* g = new TH2D(name.c_str(),"MicroMega set 1 and 4",100,-100,100,100,-100,100); //change name to string
    for(int i = 0; i < entries; i++){
      t->GetEntry(i);
      indexSum1 = 0;
      indexSum4 = 0;
      QSum1 = 0;
      QSum4 = 0;
      if (nmm4>nmm1){
        minhits = nmm1;
      }else{
        minhits = nmm4;
      }
      for (int j=0; j<minhits; j++){
        QSum1 += mm1StripQ[j];
        indexSum1 += j*mm1StripQ[j];
        QSum4 += mm4StripQ[j];
        indexSum4 += j*mm4StripQ[j];
      }
      if (!(QSum1 == 0 || QSum2 == 0)){
        clusterIndex1 = (indexSum1/QSum1) + mm1_strip_offset;
        clusterIndex4 = (indexSum4/QSum4) + mm4_strip_offset;
        clusterPosition1 = clusterIndex1 * pitch - mmWidth/2;
        clusterPosition4 = clusterIndex4 * pitch - mmWidth/2;
        g->Fill(clusterPosition1, clusterPosition4-mm4YDisplacement);
      }
    }
  }
  else if(strcmp(n,"second")==0 || strcmp(n,"2")==0){
    TH2D* g = new TH2D(name.c_str(), "MicroMega set 3 and 2",100,-100,100,100,-100,100);
    for(int i = 0; i < entries; i++){
      t->GetEntry(i);
      indexSum2 = 0;
      indexSum3 = 0;
      QSum2 = 0;
      QSum3 = 0;
      if (nmm2>nmm3){
        minhits = nmm3;
      }else{
        minhits = nmm2;
      }
      for (int j=0; j<minhits; j++){
        QSum2 += mm2StripQ[j];
        indexSum2 += j*mm2StripQ[j];
        QSum3 += mm3StripQ[j];
        indexSum3 += j*mm3StripQ[j];
      }
      if (!(QSum2 == 0 || QSum3 == 0)){
        clusterIndex2 = (indexSum2/QSum2) + mm2_strip_offset;
        clusterIndex3 = (indexSum3/QSum3) + mm3_strip_offset;
        clusterPosition2 = clusterIndex2 * pitch - mmWidth/2;
        clusterPosition3 = clusterIndex3 * pitch - mmWidth/2;
        g->Fill(clusterPosition3, clusterPosition2-mm2YDisplacement); //mm
      }
    } 
  } else {
        cout<<"MM number not recognized. Only MM sets 1 and 2 availible.\n";
        exit = true;
        break;
    }
    if(!exit){
        g->SetDirectory(0);
        if(newCanvas){
            mmScatter=new TCanvas("mmScatter","mmScatter",400,400);
            mmScatter->cd();
        }
        g->Draw("colz");
        gStyle->SetOptStat("nem");
        //redundantCanvas();
        //break;
    }  
}

void detector(int det, bool newCanvas=true){
    switch(det){
        case 1: DWCScatter("1", newCanvas);
            break;
        case 2: DWCScatter("2", newCanvas);
            break;
        case 3: DWCScatter("3", newCanvas);
            break;
        case 4: MMScatter("1", newCanvas);
            break;
        case 5: MMScatter("2", newCanvas);
            break;
        default: cout<<"Detector not found\n";
                 cout<<"Type help() for details \n";
            break;
    }
}

void trackerScatter(){
    c2 = new TCanvas("c2","c2",1500,300);
    c2->Divide(5,1);
    for(int i=1;i<6;i++){
        c2->cd(i);
        detector(i, false);    
    }
 }

void help(){
    cout<<"\tDWCScatter(\"1\")\n";
    cout<<"\tDWCScatter(\"2\")\n";
    cout<<"\tDWCScatter(\"3\")\n";
    cout<<"\tMMScatter(\"first\")\n";
    cout<<"\tMMScatter(\"second\")\n";
    cout<<"\tMMScatter(\"1\")\n";
    cout<<"\tMMScatter(\"2\")\n\n";
    cout<<"\tdetector(1)\n";
    cout<<"\t\t\t(This calls DWCScatter(\"1\")\n";
    cout<<"\tdetector(2)\n";
    cout<<"\t\t\t(This calls DWCScatter(\"2\")\n";
    cout<<"\tdetector(3)\n";
    cout<<"\t\t\t(This calls DWCScatter(\"3\")\n";
    cout<<"\tdetector(4)\n";
    cout<<"\t\t\t(This calls MMScatter(\"1\")\n";
    cout<<"\tdetector(5)\n";
    cout<<"\t\t\t(This calls MMScatter(\"2\")\n";
}
void getDisplacementsY(int _detector){
    double totalY=0, countY=0,t;
    findRuns(10000,0,false,"deflection");
    for(int i=0;i<selectedRuns;i++){
        cout<<i<<endl;
        loadAllDetectors(runSelection[i],false);
        if(getisOK(runSelection[i])){
            t=getMeansCustom(_detector)[1];
            totalY+=t;
            countY++;
        }
    }
    cout<<totalY/countY;
}
void getDisplacementsX(int _detector){
    double totalY=0, countY=0,t;
    findRuns(10000,0,false,"deflection");
    for(int i=0;i<selectedRuns;i++){
        cout<<i<<endl;
        loadAllDetectors(runSelection[i],false);
        if(getisOK(runSelection[i])){
            t=getMeansCustom(_detector)[0];
            totalY+=t;
            countY++;
        }
    }
    cout<<totalY/countY;
}
void getMeansPlaintext(int n){
    double mean[2];
    TH1D histx = TH1D(runFile, "x hits", 400, -400, 400);
    TH1D histy = TH1D(runFile, "x hits", 400, -400, 400);
    double dwcXPos, dwcYPos;
    bool toFill = true;
    switch(n){
    case 1:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc1L == 1 && ndwc1R == 1 && ndwc1U == 1 && ndwc1D == 1){
                dwcXPos = (dwc1XSlope * (int(dwc1L[0] - dwc1R[0]) + dwc1XOffset) * timeFactor)+dwc1XDisplacement;   
                dwcYPos = (dwc1YSlope * (int(dwc1U[0] - dwc1D[0]) + dwc1YOffset) * timeFactor)+dwc1YDisplacement; 
                histx.Fill(dwcXPos);
                histy.Fill(dwcYPos);
            }
        }
        cout<<"DWC1\n";
        break;
    case 2:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc2L == 1 && ndwc2R == 1 && ndwc2U == 1 && ndwc2D == 1){
                dwcXPos = (dwc2XSlope * (int(dwc2L[0] - dwc2R[0]) + dwc2XOffset) * timeFactor)+dwc2XDisplacement;   
                dwcYPos = (dwc2YSlope * (int(dwc2U[0] - dwc2D[0]) + dwc2YOffset) * timeFactor)+dwc2YDisplacement; 
                histx.Fill(dwcXPos);
                histy.Fill(dwcYPos);
            }
        }
        cout<<"DWC2\n";
        break;
    case 3:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc3L == 1 && ndwc3R == 1 && ndwc3U == 1 && ndwc3D == 1){
                dwcXPos = (dwc3XSlope * (int(dwc3L[0] - dwc3R[0]) + dwc3XOffset) * timeFactor)+dwc3XDisplacement;   
                dwcYPos = (dwc3YSlope * (int(dwc3U[0] - dwc3D[0]) + dwc3YOffset) * timeFactor)+dwc3YDisplacement; 
                histx.Fill(dwcXPos);
                histy.Fill(dwcYPos);
            }
        }
        cout<<"DWC3\n";
        break;
    case 4:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            indexSum4 = 0;
            indexSum1 = 0;
            QSum4 = 0;
            QSum1 = 0;
            if (nmm4>nmm1){
                minhits = nmm1;
            }else{
                minhits = nmm4;
            }
            for (int j=0; j<minhits; j++){
                QSum4 += mm4StripQ[j];
                indexSum4 += j*mm4StripQ[j];
                QSum1 += mm1StripQ[j];
                indexSum1 += j*mm1StripQ[j];
            }
            if (!(QSum4 == 0 || QSum1 == 0)){
                clusterIndex4 = (indexSum4/QSum4) + mm4_strip_offset;
                clusterIndex1 = (indexSum1/QSum1) + mm1_strip_offset;
                clusterPosition4 = clusterIndex4 * pitch - mmWidth/2;
                clusterPosition1 = clusterIndex1 * pitch - mmWidth/2;
                histx.Fill(clusterPosition1);
                histy.Fill(clusterPosition4-mm4YDisplacement);
            }
        }
        cout<<"MM1/MM4\n";
        break;
    case 5:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            indexSum2 = 0;
            indexSum3 = 0;
            QSum2 = 0;
            QSum3 = 0;
            if (nmm2>nmm3){
                minhits = nmm3;
            }else{
                minhits = nmm2;
            }
            for (int j=0; j<minhits; j++){
                QSum2 += mm2StripQ[j];
                indexSum2 += j*mm2StripQ[j];
                QSum3 += mm3StripQ[j];
                indexSum3 += j*mm3StripQ[j];
            }
            if (!(QSum2 == 0 || QSum3 == 0)){
                clusterIndex2 = (indexSum2/QSum2) + mm2_strip_offset;
                clusterIndex3 = (indexSum3/QSum3) + mm3_strip_offset;
                clusterPosition2 = clusterIndex2 * pitch - mmWidth/2;
                clusterPosition3 = clusterIndex3 * pitch - mmWidth/2;
                histx.Fill(clusterPosition3);
                histy.Fill(clusterPosition2-mm2YDisplacement);
            }
        }
        cout<<"MM3/MM2\n";
        break;
    default:
        cout<<"Detector not found\n";
        toFill = false;
        break;
    }
    if(toFill){
        histx.Fit("gaus");
        histy.Fit("gaus");
        mean[0] = histx.GetFunction("gaus")->GetParameter(1);
        mean[1] = histy.GetFunction("gaus")->GetParameter(1);
        cout<<"\n\n\n\nMean x = "<<mean[0];
        cout<<"\nMean y = "<<mean[1]<<"\n\n";
        //detector(n);
        //return mean;
    } else {
        //return 0;
    }
}
double* getMeansValues(int n){
    double mean[2];
    TH1D histx = TH1D(runFile, "x hits", 1000, -1000, 1000);
    TH1D histy = TH1D(runFile, "y hits", 1000, -1000, 1000);
    double dwcXPos, dwcYPos;
    bool toFill = true;
    switch(n){
    case 1:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc1L == 1 && ndwc1R == 1 && ndwc1U == 1 && ndwc1D == 1){
                dwcXPos = (dwc1XSlope * (int(dwc1L[0] - dwc1R[0]) + dwc1XOffset) * timeFactor)+dwc1XDisplacement;   
                dwcYPos = (dwc1YSlope * (int(dwc1U[0] - dwc1D[0]) + dwc1YOffset) * timeFactor)+dwc1YDisplacement; 
                histx.Fill(dwcXPos);
                histy.Fill(dwcYPos);
            }
        }
        cout<<"DWC1\n";
        break;
    case 2:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc2L == 1 && ndwc2R == 1 && ndwc2U == 1 && ndwc2D == 1){
                dwcXPos = (dwc2XSlope * (int(dwc2L[0] - dwc2R[0]) + dwc2XOffset) * timeFactor)+dwc2XDisplacement;   
                dwcYPos = (dwc2YSlope * (int(dwc2U[0] - dwc2D[0]) + dwc2YOffset) * timeFactor)+dwc2YDisplacement;
                histx.Fill(dwcXPos);
                histy.Fill(dwcYPos);
            }
        }
        cout<<"DWC2\n";
        break;
    case 3:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc3L == 1 && ndwc3R == 1 && ndwc3U == 1 && ndwc3D == 1){
                dwcXPos = (dwc3XSlope * (int(dwc3L[0] - dwc3R[0]) + dwc3XOffset) * timeFactor)+dwc3XDisplacement;   
                dwcYPos = (dwc3YSlope * (int(dwc3U[0] - dwc3D[0]) + dwc3YOffset) * timeFactor)+dwc3YDisplacement;
                histx.Fill(dwcXPos);
                histy.Fill(dwcYPos);
            } 
        }
        cout<<"DWC3\n";
        break;
    case 4:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            indexSum4 = 0;
            indexSum1 = 0;
            QSum4 = 0;
            QSum1 = 0;
            if (nmm4>nmm1){
                minhits = nmm1;
            }else{
                minhits = nmm4;
            }
            for (int j=0; j<minhits; j++){
                QSum4 += mm4StripQ[j];
                indexSum4 += j*mm4StripQ[j];
                QSum1 += mm1StripQ[j];
                indexSum1 += j*mm1StripQ[j];
            }
            if (!(QSum4 == 0 || QSum1 == 0)){
                clusterIndex4 = (indexSum4/QSum4) + mm4_strip_offset;
                clusterIndex1 = (indexSum1/QSum1) + mm1_strip_offset;
                clusterPosition4 = clusterIndex4 * pitch - mmWidth/2;
                clusterPosition1 = clusterIndex1 * pitch - mmWidth/2;
                histx.Fill(clusterPosition1);
                histy.Fill(clusterPosition4-mm4YDisplacement);
            }
        }
        cout<<"MM1/MM4\n";
        break;
    case 5:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            indexSum2 = 0;
            indexSum3 = 0;
            QSum2 = 0;
            QSum3 = 0;
            if (nmm2>nmm3){
                minhits = nmm3;
            }else{
                minhits = nmm2;
            }
            for (int j=0; j<minhits; j++){
                QSum2 += mm2StripQ[j];
                indexSum2 += j*mm2StripQ[j];
                QSum3 += mm3StripQ[j];
                indexSum3 += j*mm3StripQ[j];
            }
            if (!(QSum2 == 0 || QSum3 == 0)){
                clusterIndex2 = (indexSum2/QSum2) + mm2_strip_offset;
                clusterIndex3 = (indexSum3/QSum3) + mm3_strip_offset;
                clusterPosition2 = clusterIndex2 * pitch - mmWidth/2;
                clusterPosition3 = clusterIndex3 * pitch - mmWidth/2;
                histx.Fill(clusterPosition3);
                histy.Fill(clusterPosition2-mm2YDisplacement);
            }
        }
        cout<<"MM3/MM2\n";
        break;
    default:
        cout<<"Detector not found\n";
        toFill = false;
        break;
    }
    if(toFill){
        histx.Fit("gaus");
        histy.Fit("gaus");
        mean[0] = histx.GetFunction("gaus")->GetParameter(1);
        mean[1] = histy.GetFunction("gaus")->GetParameter(1);
        //detector(n);
        return mean;
    } else {
        return 0;
    }
}
double* getMeansCustom(int n){
    double mean[2];

    double totalX=0;
    double totalY=0;
    double count=0;

    double dwcXPos, dwcYPos;
    bool toFill = true;
    switch(n){
    case 1:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc1L == 1 && ndwc1R == 1 && ndwc1U == 1 && ndwc1D == 1){
                dwcXPos = (dwc1XSlope * (int(dwc1L[0] - dwc1R[0]) + dwc1XOffset) * timeFactor)+dwc1XDisplacement;   
                dwcYPos = (dwc1YSlope * (int(dwc1U[0] - dwc1D[0]) + dwc1YOffset) * timeFactor)+dwc1YDisplacement;
                totalX+=dwcXPos;
                totalY+=dwcYPos;
                count++;
            } 
        }
        cout<<"DWC1\n";
        break;
    case 2:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc2L == 1 && ndwc2R == 1 && ndwc2U == 1 && ndwc2D == 1){
                dwcXPos = (dwc2XSlope * (int(dwc2L[0] - dwc2R[0]) + dwc2XOffset) * timeFactor)+dwc2XDisplacement;   
                dwcYPos = (dwc2YSlope * (int(dwc2U[0] - dwc2D[0]) + dwc2YOffset) * timeFactor)+dwc2YDisplacement;
                totalX+=dwcXPos;
                totalY+=dwcYPos;
                count++;
            }
        }
        cout<<"DWC2\n";
        break;
    case 3:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if (ndwc3L == 1 && ndwc3R == 1 && ndwc3U == 1 && ndwc3D == 1){
                dwcXPos = (dwc3XSlope * (int(dwc3L[0] - dwc3R[0]) + dwc3XOffset) * timeFactor)+dwc3XDisplacement;   
                dwcYPos = (dwc3YSlope * (int(dwc3U[0] - dwc3D[0]) + dwc3YOffset) * timeFactor)+dwc3YDisplacement;
                totalX+=dwcXPos;
                totalY+=dwcYPos;
                count++;
            } 
        }
        cout<<"DWC3\n";
        break;
    case 4:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            indexSum4 = 0;
            indexSum1 = 0;
            QSum4 = 0;
            QSum1 = 0;
            if (nmm4>nmm1){
                minhits = nmm1;
            }else{
                minhits = nmm4;
            }
            for (int j=0; j<minhits; j++){
                QSum4 += mm4StripQ[j];
                indexSum4 += j*mm4StripQ[j];
                QSum1 += mm1StripQ[j];
                indexSum1 += j*mm1StripQ[j];
            }
            if (!(QSum4 == 0 || QSum1 == 0)){
                clusterIndex4 = (indexSum4/QSum4) + mm4_strip_offset;
                clusterIndex1 = (indexSum1/QSum1) + mm1_strip_offset;
                clusterPosition4 = clusterIndex4 * pitch - mmWidth/2;
                clusterPosition1 = clusterIndex1 * pitch - mmWidth/2;
                totalX+=clusterPosition1;
                totalY+=clusterPosition4-mm4YDisplacement;
                count++;
            }
        }
        cout<<"MM1/MM4\n";
        break;
    case 5:
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            indexSum2 = 0;
            indexSum3 = 0;
            QSum2 = 0;
            QSum3 = 0;
            if (nmm2>nmm3){
                minhits = nmm3;
            }else{
                minhits = nmm2;
            }
            for (int j=0; j<minhits; j++){
                QSum2 += mm2StripQ[j];
                indexSum2 += j*mm2StripQ[j];
                QSum3 += mm3StripQ[j];
                indexSum3 += j*mm3StripQ[j];
            }
            if (!(QSum2 == 0 || QSum3 == 0)){
                clusterIndex2 = (indexSum2/QSum2) + mm2_strip_offset;
                clusterIndex3 = (indexSum3/QSum3) + mm3_strip_offset;
                clusterPosition2 = clusterIndex2 * pitch - mmWidth/2;
                clusterPosition3 = clusterIndex3 * pitch - mmWidth/2;
                totalX+=clusterPosition3;
                totalY+=clusterPosition2-mm2YDisplacement;
                count++;
            }
        }
        cout<<"MM3/MM2\n";
        break;
    default:
        cout<<"Detector not found\n";
        toFill = false;
        break;
    }
    if(toFill){
        mean[0] = totalX/count;
        mean[1] = totalY/count;
        return mean;
    } else {
        return 0;
    }
}
void getHitPos(double dwc1[],double dwc2[],double dwc3[],double mm1[],double mm2[]){
    getHitPos(1,dwc1);
    getHitPos(2,dwc2);
    getHitPos(3,dwc3);
    getHitPos(4,mm1);
    getHitPos(5,mm2);
}

void getHitPos(int n, double pos[]){
    //double pos[3];
    switch(n){
    case 1:
        pos[0] = (dwc1XSlope * (int(dwc1L[0] - dwc1R[0]) + dwc1XOffset) * timeFactor)+dwc1XDisplacement;   
        pos[1] = (dwc1YSlope * (int(dwc1U[0] - dwc1D[0]) + dwc1YOffset) * timeFactor)+dwc1YDisplacement;
        pos[2] = dwc1.distance;
        break;
    case 2:
        pos[0] = (dwc2XSlope * (int(dwc2L[0] - dwc2R[0]) + dwc2XOffset) * timeFactor)+dwc2XDisplacement;   
        pos[1] = (dwc2YSlope * (int(dwc2U[0] - dwc2D[0]) + dwc2YOffset) * timeFactor)+dwc2YDisplacement;
        pos[2] = dwc2.distance;
        break;
    case 3:
        pos[0] = (dwc3XSlope * (int(dwc3L[0] - dwc3R[0]) + dwc3XOffset) * timeFactor)+dwc3XDisplacement;   
        pos[1] = (dwc3YSlope * (int(dwc3U[0] - dwc3D[0]) + dwc3YOffset) * timeFactor)+dwc3YDisplacement;
        pos[2] = dwc3.distance;
        break;
    case 4:
        indexSum4 = 0;
        indexSum1 = 0;
        QSum4 = 0;
        QSum1 = 0;
        if (nmm4>nmm1){
            minhits = nmm1;
        }else{
            minhits = nmm4;
        }
        for (int j=0; j<minhits; j++){
            QSum4 += mm4StripQ[j];
            indexSum4 += j*mm4StripQ[j];
            QSum1 += mm1StripQ[j];
            indexSum1 += j*mm1StripQ[j];
        }
        if (!(QSum4 == 0 || QSum1 == 0)){
            clusterIndex4 = (indexSum4/QSum4) + mm4_strip_offset;
            clusterIndex1 = (indexSum1/QSum1) + mm1_strip_offset;
            clusterPosition4 = clusterIndex4 * pitch - mmWidth/2;
            clusterPosition1 = clusterIndex1 * pitch - mmWidth/2;
            pos[0] = clusterPosition1;
            pos[1] = clusterPosition4-mm4YDisplacement;
            pos[2] = mm1.distance;
        }
        break;
    case 5:
        indexSum2 = 0;
        indexSum3 = 0;
        QSum2 = 0;
        QSum3 = 0;
        if (nmm2>nmm3){
            minhits = nmm3;
        }else{
            minhits = nmm2;
        }
        for (int j=0; j<minhits; j++){
            QSum2 += mm2StripQ[j];
            indexSum2 += j*mm2StripQ[j];
            QSum3 += mm3StripQ[j];
            indexSum3 += j*mm3StripQ[j];
        }
        if (!(QSum2 == 0 || QSum3 == 0)){
            clusterIndex2 = (indexSum2/QSum2) + mm2_strip_offset;
            clusterIndex3 = (indexSum3/QSum3) + mm3_strip_offset;
            clusterPosition2 = clusterIndex2 * pitch - mmWidth/2;
            clusterPosition3 = clusterIndex3 * pitch - mmWidth/2;
            pos[0] = clusterPosition3;
            pos[1] = clusterPosition2-mm2YDisplacement;
            pos[2] = mm3.distance;
        }
        break;
    default:
        cout<<"Detector not found\n";
        break;
    }
    //if(pos[0]!=NULL){
        pos[0]/=1000.0;
        pos[1]/=1000.0;
        //return pos;
    //} else {
      //  return NULL;
    //}
}
bool detectedByAll(int n = 3){
    bool d1=false, d2=false, d3=false;
    if(ndwc3L == 1 && ndwc3R == 1 && ndwc3U == 1 && ndwc3D == 1){
        d3=true;
    }
    if(ndwc2L == 1 && ndwc2R == 1 && ndwc2U == 1 && ndwc2D == 1){
        d2=true;
    }
    if(ndwc1L == 1 && ndwc1R == 1 && ndwc1U == 1 && ndwc1D == 1){
        d1=true;
    }
    switch (n){
        case 1:return d1; break;
        case 2:return (d1&&d2); break;
        case 3:return (d1&&d2&&d3); break;
        default: cout<<"No detector found, choose DWCs 1,2,3\n"; 
            return NULL; break;
    }
}
// void redundantCanvas(){

        //NOTE: This issue has been resolved by using hist->SetDirectory(0);

//       The following code attempts to disconnect the root file to the last plot.
//       It does this by pushing t (pointer) to a new redundant canvas.
//       When the file is closed, which is done before opening a new one,
//       the pointer associated with that file gets deleted too, thus the
//       MM3/MM2 scatter plot also gets deleted exclusively (the other 4 stay).
//       This is probably the worst way to do it. Good luck to the maintainer
//       of this code. There is probably an easier, safer, more efficient, and 
//       less stupid way of doing it.
//       It is probably mentioned in the documentation. Alas, that is for you.
//       This code works for now, but I assure you,
//       ###### SOMETHING WILL DEFINITELY BREAK ######
    
//     c3=new TCanvas(false);
//     c3->SetName("c3");
//     c3->SetTitle("c3");
//     c3->cd();
//     c3->Draw();
//     detector(1); //pushes t to new pad
//     c3->~TCanvas();
//}

//AODGraphs.h
//06-03-2019

void drawAngleOfDeviationX(){
    if(!file.IsOpen()){
        cout<<"No file loaded.\n";
    } else { 
        aodHist = new TH1D(name.c_str(), "Angle of Deviation (XZ plane) in radians;Radians;Count", 250, -0.45, 0.45);
        double dwc1pos[3]={0,0,0};
        double dwc2pos[3]={0,0,0};
        double dwc3pos[3]={0,0,0};
        double mm1pos[3]={0,0,0};
        double mm2pos[3]={0,0,0};
        double init=0, final=0, angle=0;
        for(int i = 0; i < entries; i++){
            t->GetEntry(i);
            if(detectedByAll(2)){
                getHitPos(dwc1pos, dwc2pos, dwc3pos, mm1pos, mm2pos);
                init = atan((dwc2pos[x]-dwc1pos[x])/(dwc2pos[z]-dwc1pos[z]));
                final = atan((mm2pos[x]-mm1pos[x])/(mm2pos[z]-mm1pos[z]));
                angle = final-init;
                aodHist->Fill(angle);
            }
        }
        aodCanvas = new TCanvas("aodCanvas","aodCanvas",700,500);
        aodCanvas->cd();
        aodHist->Draw();
        aodHist->Fit("gaus");
        gStyle->SetOptStat("nem");
        aodHist->SetDirectory(0);
    }
}

void drawAODGraphs(string graphType=""){
    if(selectedRuns!=0){
        // if (!AODMulti){
        //  TCanvas *AODMulti;
        // } 
        // if(aodHist){
        //  TH2D *aodHist;
        // }
        // if(graphType!=""){

        // }
        if(graphType=="momentum"){
            aodHist = new TH2D("Momentum vs Deviation",";Momentum;Radians",1000,-12000,12000,1000,-0.04,0.04);
        }
        if(graphType=="momentumabs"){
            aodHist = new TH2D("Momentum vs Deviation",";Momentum;Radians",1000,-1000,11000,1000,-0.01,0.04);
        }
        if(graphType=="amps"){
            aodHist = new TH2D("Amps vs Deviation",";Amps;Radians",270,-10,260,1000,-0.04,0.04);
        }
        cout<<endl;
        double AODMean[maxRuns];
        clearSelection(AODMean);
        int badMeans = 0;
        for(int i=0;i<selectedRuns;i++){
            loadAllDetectors(runSelection[i], false);
            cout<<"Current Selection Index: "<<i<<endl;
            if(!getisOK(runSelection[i])){
                badMeans++;
                cout<<"###Bad run###"<<endl;
                cout<<"Conditions: "<<name.c_str()<<endl;
                cout<<"Run:        "<<runSelection[i]<<endl<<endl;
                continue;
            } else if(getType(runSelection[i])!="deflection"){
                cout<<"###"<<getType(runSelection[i])<<"###"<<endl;
            }
            else {
                AODMean[i]=AODMean();
                if(graphType=="momentum"){
                    aodHist->Fill(momentum*1000,AODMean[i]);
                    aodHist->SetTitle((ampsString.str()+"Amps").c_str());
                }
                if(graphType=="momentumabs"){
                    aodHist->Fill(fabs(momentum*1000),fabs(AODMean[i]));
                    aodHist->SetTitle((ampsString.str()+"Amps"+" (absolute values)").c_str());
                }
                if(graphType=="amps"){
                    aodHist->Fill(amps, fabs(AODMean[i]));
                    aodHist->SetTitle((momentumString.str()+"GeV/c").c_str());
                }
            }
            cout<<"Conditions: "<<name.c_str()<<endl;
            cout<<"Run:        "<<runSelection[i]<<endl;
            cout<<"Mean:       "<<AODMean[i]<<endl<<endl;
        }
        if(graphType!=""){
            TCanvas *AODMulti=new TCanvas("AODMulti","AODMulti",400,400);
            //AODMulti.SetWindowSize(400,400);
            AODMulti->cd();
            AODMulti->Draw();
            aodHist->Draw("box");
            
            if(graphType=="amps"){  
                aodHist->Fit("pol1","c");
            }
            if(graphType=="momentum" || graphType=="momentumabs"){
                f1 = new TF1("f1","[0]/x+[1]",aodHist->GetXaxis()->GetXmin(),aodHist->GetXaxis()->GetXmax());
                aodHist->Fit("f1","R");
            }

            gStyle->SetOptStat("ne");
            aodHist->SetDirectory(0);
            drawAODTheoretical(graphType);
        }
    } else {
        cout<<"No Runs Selected"<<endl;
        cout<<"Add runs using findRuns()"<<endl;
    }
}

double AODMean(){
    double dwc1pos[3]={0,0,0};
    double dwc2pos[3]={0,0,0};
    double dwc3pos[3]={0,0,0};
    double mm1pos[3]={0,0,0};
    double mm2pos[3]={0,0,0};
    double init=0, final=0, angle=0;
    double mean=0, total=0;
    for(int i = 0; i < entries; i++){
        t->GetEntry(i);
        if(detectedByAll(2)){
            getHitPos(dwc1pos, dwc2pos, dwc3pos, mm1pos, mm2pos);
            init = atan((dwc2pos[x]-dwc1pos[x])/(dwc2pos[z]-dwc1pos[z]));
            final = atan((mm2pos[x]-mm1pos[x])/(mm2pos[z]-mm1pos[z]));
            angle = final-init;
            mean+=angle;
            total++;
        }
    }
    mean/=total;
    return mean;
}
double AODMean(bool something){
    //This is an old version
    double init=0, final=0, value=0;
    int totalEntries=0;
    for(int i = 0; i < entries; i++){
        //cout<<".";
        t->GetEntry(i);
        //value = 0;
        indexSum3 = 0;
        QSum3 = 0;
        indexSum1 = 0;
        QSum1 = 0;
        if (nmm3>nmm1){
          minhits = nmm1;
        }else{
          minhits = nmm3;
        }
        for (int j=0; j<minhits; j++){
          QSum3 += mm3StripQ[j];
          indexSum3 += j*mm3StripQ[j];
          QSum1 += mm1StripQ[j];
          indexSum1 += j*mm1StripQ[j];
        }
        if (!(QSum3==0 || QSum1 ==0)){
          clusterIndex3 = (indexSum3/QSum3) + mm3_strip_offset;
          clusterIndex1 = (indexSum1/QSum1) + mm1_strip_offset;
          clusterPosition3 = clusterIndex3 * pitch - mmWidth/2;
          clusterPosition1 = clusterIndex1 * pitch - mmWidth/2;
          //cout<<"Cluster 3 - Cluster 1"<<clusterPosition3-clusterPosition1<<endl;
          if (ndwc1L == 1 && ndwc1R == 1 && ndwc2L == 1 && ndwc2R == 1){ 
            dwc1XPos = ((dwc1XSlope * (int(dwc1L[0] - dwc1R[0]) + dwc1XOffset) * timeFactor)+dwc1XDisplacement);
            dwc2XPos = ((dwc2XSlope * (int(dwc2L[0] - dwc2R[0]) + dwc2XOffset) * timeFactor)+dwc2XDisplacement);
            //dwc3XPos = ((dwc3Slope * (int(dwc3L[0] - dwc3R[0]) + dwc3Offset) * timeFactor)+dwc3Displacement);  
            init=atan((dwc2XPos-dwc1XPos)/(dwc2.distance*1000-dwc1.distance*1000));
            final=atan((clusterPosition3-clusterPosition1)/(mm3.distance*1000-mm1.distance*1000));
            //cout<<final;
            //final=findAngle(dwc3XPos, clusterPosition1, clusterPosition3); //defined in crypticontics.h
            value=value+(final-init);
            //cout<<value<<endl;
            totalEntries++;
            //cout<<totalEntries<<endl;
            }
        }
    }
    cout<<endl;
    //cout<<totalEntries<<endl;
    return (value/double(totalEntries));
}
double AODSigma(){
    double dwc1pos[3]={0,0,0};
    double dwc2pos[3]={0,0,0};
    double dwc3pos[3]={0,0,0};
    double mm1pos[3]={0,0,0};
    double mm2pos[3]={0,0,0};

    double initial, final, angle;
    
    double mean=AODMean();
    double sigma=0, total=0;
    
    for(int i = 0; i < entries; i++){
        t->GetEntry(i);
        if(detectedByAll(2)){
            getHitPos(dwc1pos, dwc2pos, dwc3pos, mm1pos, mm2pos);
            final = (mm2pos[x]-mm1pos[x]);
            initial = (dwc2pos[x]-dwc1pos[x]);
            //cout<<initial<<endl;
            initial = atan((dwc2pos[x]-dwc1pos[x])/(dwc2pos[z]-dwc1pos[z]));
            final = atan((mm2pos[x]-mm1pos[x])/(mm2pos[z]-mm1pos[z]));
            angle=final-initial;
            sigma += pow((angle-mean),2);
            total++;
        }
    }
    cout<<total<<endl;
    sigma/=total;
    return sqrt(sigma);
}
void drawAODTheoretical(string graphType=""){
    //very bad code. please rewrite. I couldn't bother. -SC
    if(graphType=="amps"){
        ampHist = new TH2D("Amps vs Deviation","Theoretical Amps vs Deviation;Amps;Radians",270,-10,260,1000,-0.04,0.04);
        cout<<"\nTheoretical: \n";
        cout<<"60A:  "<<fabs(angleOfDeviation(beam.proton, beam, mdx27, 60))<<endl;
        cout<<"120A: "<<fabs(angleOfDeviation(beam.proton, beam, mdx27, 120))<<endl;
        cout<<"180A: "<<fabs(angleOfDeviation(beam.proton, beam, mdx27, 180))<<endl;
        cout<<"240A: "<<fabs(angleOfDeviation(beam.proton, beam, mdx27, 240))<<endl;

        ampHist->Fill(60, fabs(angleOfDeviation(beam.proton, beam, mdx27, 60)));
        ampHist->Fill(120, fabs(angleOfDeviation(beam.proton, beam, mdx27, 120)));
        ampHist->Fill(180, fabs(angleOfDeviation(beam.proton, beam, mdx27, 180)));
        ampHist->Fill(240, fabs(angleOfDeviation(beam.proton, beam, mdx27, 240)));

        //ampHist->Draw("SAME");
        ampHist->Fit("pol1");
        ampHist->GetFunction("pol1")->SetLineColor(kGreen);
        gStyle->SetOptStat("ne");
        ampHist->SetDirectory(0);
    }
    if(graphType=="momentum"){
        momHist = new TH2D("Momentum vs Deviation","Theoretical Momentum vs Deviation;Momentum;Radians",1000,-12000,12000,1000,-0.04,0.04);
        
        cout<<"\nTheoretical: \n";
        cout<<"-10 GeV/c: "<<(angleOfDeviation(Beam(-10.0).proton, Beam(-10.0), mdx27, amps))<<endl;
        cout<<"-5 GeV/c:  "<<(angleOfDeviation(Beam(-5.0).proton, Beam(-5.0), mdx27, amps))<<endl;
        cout<<" 5 GeV/c:  "<<(angleOfDeviation(Beam(5.0).proton, Beam(5.0), mdx27, amps))<<endl;
        cout<<" 10 GeV/c: "<<(angleOfDeviation(Beam(10.0).proton, Beam(10.0), mdx27, amps))<<endl;

        momHist->Fill(-10000, (angleOfDeviation(Beam(-10.0).proton, Beam(-10.0), mdx27, amps)));
        momHist->Fill(-5000, (angleOfDeviation(Beam(-5.0).proton, Beam(-5.0), mdx27, amps)));
        momHist->Fill(5000, (angleOfDeviation(Beam(5.0).proton, Beam(5.0), mdx27, amps)));
        momHist->Fill(10000, (angleOfDeviation(Beam(10.0).proton, Beam(10.0), mdx27, amps)));

        f2 = new TF1("f2","[0]/x+[1]",momHist->GetXaxis()->GetXmin(), momHist->GetXaxis()->GetXmax());
        momHist->Draw("box SAME");
        momHist->Fit("f2","R");
        momHist->GetFunction("f2")->SetLineColor(kGreen);
        gStyle->SetOptStat("ne");
        momHist->SetDirectory(0);

    }
    if(graphType=="momentumabs"){
        momHist = new TH2D("Momentum vs Deviation (absolute)","Theoretical Momentum vs Deviation (absolute);Momentum;Radians",1000,-1000,11000,1000,-0.01,0.04);
        
        cout<<"\nTheoretical: \n";
        cout<<"-10 GeV/c: "<<fabs(angleOfDeviation(Beam(-10.0).proton, Beam(-10.0), mdx27, amps))<<endl;
        cout<<"-5 GeV/c:  "<<fabs(angleOfDeviation(Beam(-5.0).proton, Beam(-5.0), mdx27, amps))<<endl;
        cout<<" 5 GeV/c:  "<<fabs(angleOfDeviation(Beam(5.0).proton, Beam(5.0), mdx27, amps))<<endl;
        cout<<" 10 GeV/c: "<<fabs(angleOfDeviation(Beam(10.0).proton, Beam(10.0), mdx27, amps))<<endl;

        momHist->Fill(-10000, fabs(angleOfDeviation(Beam(-10.0).proton, Beam(-10.0), mdx27, amps)));
        momHist->Fill(-5000, fabs(angleOfDeviation(Beam(-5.0).proton, Beam(-5.0), mdx27, amps)));
        momHist->Fill(5000, fabs(angleOfDeviation(Beam(5.0).proton, Beam(5.0), mdx27, amps)));
        momHist->Fill(10000, fabs(angleOfDeviation(Beam(10.0).proton, Beam(10.0), mdx27, amps)));

        f2 = new TF1("f2","[0]/x+[1]",momHist->GetXaxis()->GetXmin(), momHist->GetXaxis()->GetXmax());
        momHist->Draw("box SAME");
        momHist->Fit("f2","R");
        momHist->GetFunction("f2")->SetLineColor(kGreen);
        gStyle->SetOptStat("ne");
        momHist->SetDirectory(0);

    }
}

void drawAODGraphs3D(){
    findRuns(5000,'+',false,"deflection");
    appendRunSelection(-5000,'+',false,"deflection");

    appendRunSelection(-10000,'+',false,"deflection");
    appendRunSelection(10000,'+',false,"deflection");
    
    appendRunSelection(-8750,'+',false,"deflection");
    appendRunSelection(8750,'+',false,"deflection");
    
    appendRunSelection(-7500,'+',false,"deflection");
    appendRunSelection(7500,'+',false,"deflection");
    
    appendRunSelection(-6250,'+',false,"deflection");
    appendRunSelection(6250,'+',false,"deflection");
    
    appendRunSelection(-3500,'+',false,"deflection");
    appendRunSelection(3500,'+',false,"deflection");
    
    cout<<selectedRuns<<endl;
    if(selectedRuns!=0){
        aod3DHist = new TH3F();
        aod3DHist->SetName("Runs");
        aod3DHist->SetTitle("Momentum vs Amps vs Deviation;Momentum;Amps;Radians");
        aod3DHist->SetBins(1000,-12000,12000,27,-10,260,100,-0.04,0.04);
        for(int i=0;i<selectedRuns;i++){
            cout<<"Current Selection Index: "<<i<<endl;
            double mean=0;
            if(!getisOK(runSelection[i])){
                cout<<"###Bad run###"<<endl;
                cout<<"Conditions: "<<name.c_str()<<endl;
                cout<<"Run:        "<<runSelection[i]<<endl<<endl;
                continue;
            } else if(getType(runSelection[i])!="deflection"){ cout<<"###"<<getType(runSelection[i])<<"###"<<endl; }
            
            else {
                loadAllDetectors(runSelection[i], false);
                mean = (AODMean()); 
                aod3DHist->Fill(momentum*1000, amps, mean); 
            }
                cout<<"Conditions: "<<name.c_str()<<endl;
                cout<<"Run:        "<<runSelection[i]<<endl;
                cout<<"Mean:       "<<mean<<endl<<endl;
        }
            aod3DHist->Draw("cm(0, kWHITE,10,10,1)");
            gStyle->SetOptStat("ne");
            aod3DHist->SetDirectory(0);
            //f2=new TF2("f2","[0]*(y/x)+[1]",aod3DHist->GetXaxis()->GetXmin(), aod3DHist->GetXaxis()->GetXmax(),aod3DHist->GetYaxis()->GetXmin(), aod3DHist->GetYaxis()->GetXmax());
            //aod3DHist->Fit("f2","R");
    } else {
        cout<<"No Runs Selected"<<endl;
        cout<<"Add runs using findRuns()"<<endl;
    }

}
void calculateZero(){
    findRuns(5000,0,false,"deflection");
    appendRunSelection(-5000,0,false,"deflection");

    // appendRunSelection(-10000,0,false,"deflection");
    // appendRunSelection(10000,0,false,"deflection");
    
    // appendRunSelection(-8750,0,false,"deflection");
    // appendRunSelection(8750,0,false,"deflection");
    
    // appendRunSelection(-7500,0,false,"deflection");
    // appendRunSelection(7500,0,false,"deflection");
    
    // appendRunSelection(-6250,0,false,"deflection");
    // appendRunSelection(6250,0,false,"deflection");
    
    // appendRunSelection(-3500,0,false,"deflection");
    // appendRunSelection(3500,0,false,"deflection");

    cout<<"Total Runs: "<<selectedRuns<<endl;
    double total=0;
    for(int i=0;i<selectedRuns;i++){
        if(!getisOK(runSelection[i])){continue;}
        else{
            loadAllDetectors(runSelection[i], false);
            total+=AODMean();
            cout<<i<<endl;
        }
    }
    cout<<total/double(selectedRuns);
}


void drawAmps(){
    if(selectedRuns!=0){
        std::vector<double> AODMeans;
        std::vector<double> AODSigmas;
        std::vector<double> xs;
        std::vector<double> xe;
        double badRuns=0;
        for(int i=0;i<selectedRuns;i++){
            loadAllDetectors(runSelection[i],false);
            cout<<"Current Selection Index: "<<i<<endl;
            if(!getisOK(runSelection[i])){
                badRuns++;
                cout<<"###Bad run###"<<endl;
                cout<<"Conditions: "<<name.c_str()<<endl;
                cout<<"Run:        "<<runSelection[i]<<endl<<endl;
            }
            else {
                AODMeans.push_back(AODMean());
                AODSigmas.push_back(AODSigma());
                xs.push_back(amps);
                xe.push_back(0);
            cout<<"Conditions: "<<name.c_str()<<endl;
            cout<<"Run:        "<<runSelection[i]<<endl;
            cout<<"Mean:       "<<AODMeans.at(i-badRuns)<<endl;
            cout<<"Sigma:      "<<AODSigmas.at(i-badRuns)<<endl<<endl;
        }
    }
        TCanvas *AODMulti=new TCanvas("AODMulti","AODMulti",400,400);
        //AODMulti.SetWindowSize(400,400);
        AODMulti->cd();
        AODMulti->Draw();
        int n = xs.size();
        double a[11],b[11],c[11],d[11];
        for(int i=0;i<n;i++){
            a[i]=xs.at(i);
            b[i]=AODMeans.at(i);
            c[i]=xe.at(i);
            d[i]=AODSigmas.at(i);
        }
        //ampGraph = new TGraphErrors(xs.size(),xs,AODMeans,xe,AODSigmas);
        ampGraph = new TGraphErrors(n,a,b,c,d);
            gStyle->SetOptStat("ne");
            ampGraph->Draw("ALP");
            ampGraph->Fit("pol1");
            //ampGraph->SetDirectory(0);
        
    } else {
        cout<<"No Runs Selected"<<endl;
        cout<<"Add runs using findRuns()"<<endl;
    }
}
