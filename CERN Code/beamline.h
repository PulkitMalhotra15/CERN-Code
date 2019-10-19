 //17-04-2019

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
    void updateData(string type, double data){
        bool noType = false;
        if(type=="strength"){
            strength = data;
        }
        else if(type == "width"){
            width = data;
        }
        else if(type == "distance"){
            distance = data;
        }
        else {
            cout<<"No member called "<<type<<endl;
            noType=true;
        }
        if(!noType){
            type[0] = toupper(type[0]);
            cout<<type<<" updated to "<<data<<endl;
        }
    }
    double getBP(){  //source: Cristavao
        double _amps = strength*240;
        double bp = 0;
        switch (_amps){
            case 0:   bp = 0; break;
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
                if(_amps > 9){
                    if(_amps < 260){
                        cout<<"Using bending power for "<<floor(_amps/10)*10<<" Amperes\n";
                        return getBP(floor(_amps/10)*10);
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
    void display(){
        cout<<"Strength:      "<<strength<<"T"<<endl;
        cout<<"Bending Power: "<<getBP()<<"Tm"<<endl;
        cout<<"Width:         "<<width<<"m"<<endl;
        cout<<"Distance:      "<<distance<<"m"<<endl;
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
    //return asin((m.getBP()*p.charge)/b.momentum.SI);
    return asin((Magnet(amps/240.0).getBP()*p.charge)/b.momentum.SI);
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
    return (b.momentum.SI*sin(angle)/(Magnet(amps/240.0).getBP()));
}
//(3)
double findCharge(double angleX, double angleY, Beam b, double amps, bool a){
    return findCharge(getZAngle(angleX,angleY), b, amps); //calls upon (2), may be redundant
}
//(4)
double findCharge(double angleX, double angleY, Beam b, double amps){ //Aayush derived this one
    return ((sin(angleX)*b.momentum.SI)/(Magnet(amps/240.0).getBP()*cos(angleY)));
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
// double getBP(int amps){  //source: CristavaO
//     MOVED TO CLASS MAGNET ON 16-04-2019
//     //cout<<"Obsolete function. Use Magnet().getBP() instead."<<endl;
//     double bp = 0;
//     switch (amps){
//         case 10:  bp = 0.022521; break;
//         case 20:  bp = 0.045042; break;   
//         case 30:  bp = 0.067563; break;
//         case 40:  bp = 0.090084; break;
//         case 50:  bp = 0.112605; break;
//         case 60:  bp = 0.135126; break;
//         case 70:  bp = 0.157648; break;
//         case 80:  bp = 0.180169; break;
//         case 90:  bp = 0.202690; break;
//         case 100: bp = 0.225211; break;
//         case 110: bp = 0.247732; break;
//         case 120: bp = 0.270253; break;
//         case 130: bp = 0.292774; break;
//         case 140: bp = 0.315295; break;
//         case 150: bp = 0.337816; break;
//         case 160: bp = 0.360337; break;
//         case 170: bp = 0.382534; break;
//         case 180: bp = 0.403731; break;
//         case 190: bp = 0.423909; break;
//         case 200: bp = 0.443067; break;
//         case 210: bp = 0.461204; break;
//         case 220: bp = 0.478322; break;
//         case 230: bp = 0.494420; break;
//         case 240: bp = 0.509498; break;
//         case 250: bp = 0.523556; break;
//         default: 
//             cout<<"Please input amps as multiples of 10 only, range: 10 to 250 Amperes\n";
//             if(amps > 9){
//                 if(amps < 260){
//                     cout<<"Using bending power for "<<floor(amps/10)*10<<" Amperes\n";
//                     return getBP(floor(amps/10)*10);
//                 }
//                 else{
//                     cout<<"Using bending power for 250 Amperes\n";
//                     return getBP(250);
//                 }
//             }
//             break;
//     }
//     return bp;
// }
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
    v = fabs(v); //because momentum can be negative so it's sometimes negative speed
    return v;
}

double findTime(double speed, double distance){
    return distance/speed;
}

// void autoAssign(){
// 	beam=Beam(1,'+');
// 	//magnet=Magnet(1.0);
// 	magnet=Magnet("MDX27",0.5);
// 	mm=Detector("MicroMega",1);
// 	cout<<"Assigned 1 GeV/c Beam beam, 0.5T/0.55m Magnet magnet (equiv to MDX27), 1m away Detector mm (MicroMega) \n";
// }


class Help{
	public:
	Help(){}
	void Beam(){
		cout<<"To assign Beam\n";
		cout<<"	Expecting: \n";
		cout<<"		Beam() \n";
		cout<<"		Beam(double Momentum(GeV))\n";
		cout<<"		Beam(double Momentum(GeV), POSITIVE *or* NEGATIVE)\n";
		cout<<"		Beam(double Momentum(GeV), '+' *or* '-')\n";
	}
	void Magnet(){
		cout<<"To assign Magnet\n";
		cout<<"		Expecting: \n";
		cout<<"		Magnet(double strength)\n";
	}
	void Particle(){
		cout<<"To assign Particle\n";
		cout<<"		Expecting: \n";
		cout<<"		Particle(char name[], double mass, double charge, double spin)\n";
	}
	void lorentzRadius(){
		cout<<"To execute lorentzRadius\n";
		cout<<"	Expecting:\n";
		cout<<"	lorentzRadius(Particle p, Beam b, Magnet m)\n";
	}
	void angleOfDeviation(){
		cout<<"To execute angleOfDeviation \n";
		cout<<"	Expecting: \n";
		cout<<"	angleOfDeviation(Particle p, Beam b, Magnet m) \n";
		cout<<"	angleOfDeviation(double deviation, Magnet m, Detector mm, int rootNumber)\n";
	}
	void lateralDeviation(){
		cout<<"To execute lateralDeviation \n";
		cout<<"	Expecting:\n";
		cout<<"	lateralDeviation(Particle p, Beam b, Magnet m, Detector mm)";
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

