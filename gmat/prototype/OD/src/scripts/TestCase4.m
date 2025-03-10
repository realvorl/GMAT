global TestCase
TestCase = 3;

%==========================================================================
%==========================================================================
%------  Prepare GMAT creates the sandbox, and sets path data
%==========================================================================
%==========================================================================
PrepareGMAT

%==========================================================================
%==========================================================================
%------  Define the spacecraft properties
%==========================================================================
%==========================================================================
ODSat       = Spacecraft();
ODSat.Id    = 21639;
ODSat.Epoch = 24228.72771990741;
ODSat.X     = 9882.164071524565;
ODSat.Y     = -23;
ODSat.Z     = 1837.579337039707;
ODSat.VX    = 0;
ODSat.VY    = 6.233189510799131;
ODSat.VZ    = 0.8480529946665489;
ODSat.DryMass = 1703.6700;
ODSat.OrbitCovariance = diag([100000^2*ones(3,1);1000^2*ones(3,1)]);
ODSat.Cr      = 2.2;
ODSat.Cd      = 1.8;

%==========================================================================
%==========================================================================
%------  Define the batch least squares solver
%==========================================================================
%==========================================================================
MauiData = GroundStationMeasurement();
MauiData.Filename       = 'LEOMaui2Stations.mat';
MauiData.AddDataType{1} = {'Range','ODSat','Maui'};
MauiData.AddDataType{2} = {'Range','ODSat','NewGS'};

%==========================================================================
%==========================================================================
%------  Define the batch least squares solver
%==========================================================================
%==========================================================================
BLS                 = BatchEstimator();
BLS.MaxIterations   = 10;
BLS.RelTolerance    = 1e-5;
BLS.AbsTolerance    = 1e-5;
BLS.Measurements    = {'MauiData'};
BLS.SolveFor        = {'ODSat.CartesianState','MauiData.RangeMeas.Bias', 'Maui.Location'};
BLS.Propagator      = 'ODProp';
 
%==========================================================================
%==========================================================================
%-----  Define the ground station properties    
%==========================================================================
%==========================================================================
Maui    = GroundStation();
Maui.Id = 222;
Maui.X  = -4450.8;
Maui.Y  =  2676.1;
Maui.Z  = -3691.38 ;

NewGS = GroundStation();
NewGS.Id = 223;
NewGS.X  = 6378*cos(0.785398163397448)*sin(0.785398163397448);
NewGS.Y  = 6378*cos(0.785398163397448)*cos(0.785398163397448);;
NewGS.Z  = 6378*sin(0.785398163397448);

%==========================================================================
%==========================================================================
%-----  Define the Propagator   
%==========================================================================
%==========================================================================
ODProp = Propagator();
ODProp.FM.CentralBody  = 'Earth';
ODProp.FM.PointMasses  = {'Earth'};
ODProp.FM.SRP          = 'Off';
ODProp.Type            = 'RungeKutta89';
ODProp.InitialStepSize = 60;
ODProp.Accuracy        = 1.0e-8;

%==========================================================================
%==========================================================================
%------  RunGMAT initializes Sandbox and Runs the mission
%==========================================================================
%==========================================================================

global ObjectStore jd_ref;

%==========================================================================
%==========================================================================
%------  Intialize the Solar System
%==========================================================================
%==========================================================================

SolarSystem = SolarSystem();
SolarSystem.Initialize;

%==========================================================================
%==========================================================================
%------  Intialize the sandbox and all objects in the sandbox
%==========================================================================
%==========================================================================

%---- Hard code initializing objects into Object Store.  Eventually this
%     should be done in an automated way.
SandBox.AddObject(ODSat,'ODSat');
SandBox.AddObject(BLS,'BLS');
SandBox.AddObject(Maui,'Maui');
SandBox.AddObject(MauiData,'MauiData');
SandBox.AddObject(ODProp,'ODProp');
SandBox.AddObject(NewGS,'NewGS');
SandBox.Initialize(SolarSystem);

%==========================================================================
%==========================================================================
%------  Intialize the commands
%==========================================================================
%==========================================================================

RunEst = RunEstimator();
RunEst.Initialize(BLS);
RunEst.Execute();
grid on;
