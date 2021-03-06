//Parameters list for beta prime precipitation evolution problem

//define problem dimensions
#define problemDIM 3
#define spanX 8.0
#define spanY 8.0
#define spanZ 8.0

//define mesh parameters
#define subdivisionsX 1
#define subdivisionsY 1
#define subdivisionsZ 1
#define refineFactor 4
#define finiteElementDegree 1

//define time step parameters
#define timeStep 6.0e-4
#define timeIncrements 1000
#define timeFinal 100000000
#define skipImplicitSolves 1

//define solver parameters
#define solverType SolverCG
#define relSolverTolerance 1.0e-8
#define absSolverTolerance 1.0e-8
#define abs_tol false
#define maxSolverIterations 10000

//define results output parameters
#define writeOutput true
#define skipOutputSteps 1000 //(timeIncrements/10)

// flag to allow or disallow nucleation
#define nucleation_occurs false

#define num_sop 3
#define numFields (1+num_sop+problemDIM)

//define Cahn-Hilliard parameters (No Gradient energy)
#define McV 1.0

//define Allen-Cahn parameters
#define Mn1V 50.0
#define Mn2V 50.0
#define Mn3V 50.0

double Kn1[3][3]={{1.0,0,0},{0,0.5,0},{0,0,1.0}};
double Kn2[3][3]={{1.0,0,0},{0,0.5,0},{0,0,1.0}};
double Kn3[3][3]={{1.0,0,0},{0,0.5,0},{0,0,1.0}};

//define energy barrier coefficient (used to tune the interfacial energy)
#define W -1.0

//define Mechanical properties
#define n_dependent_stiffness false
#define MaterialModelV ANISOTROPIC
#define MaterialConstantsV {31.3,31.3,32.45,6.65,6.65,9.15,13.0,10.45,0,0,0,10.45,0,0,0,0,0,0,0,0,0} //scaled by E* = 2e9 J/m^3

// Stress-free transformation strains
// Linear fits for the stress-free transformation strains in for sfts = sfts_linear * c + sfts_const
double sfts_linear1[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
double sfts_const1[3][3] = {{0.1305,0,0},{0,-0.0152,0},{0,0,-0.014}}; //Mg-Nd beta-prime

double sfts_linear2[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
double sfts_const2[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

double sfts_linear3[3][3] = {{0,0,0},{0,0,0},{0,0,0}};
double sfts_const3[3][3] = {{0,0,0},{0,0,0},{0,0,0}};

// Used in n=1 phase if n_dependent_stiffness == true
#define MaterialModelBetaV ISOTROPIC
#define MaterialConstantsBetaV {2.0,0.3}

//define free energy expressions (Mg-Nd data from CASM)
#define faV (24.7939*c*c - 1.6752*c - 1.9453e-06)
#define facV (49.5878*c - 1.6752)
#define faccV (49.5878)
#define fbV (37.9316*c*c - 10.7373*c + 0.5401)
#define fbcV (75.8633*c - 10.7373)
#define fbccV (75.8633)

#define h1V (3.0*n1*n1-2.0*n1*n1*n1)
#define h2V (3.0*n2*n2-2.0*n2*n2*n2)
#define h3V (3.0*n3*n3-2.0*n3*n3*n3)
#define hn1V (6.0*n1-6.0*n1*n1)
#define hn2V (6.0*n2-6.0*n2*n2)
#define hn3V (6.0*n3-6.0*n3*n3)

#define h1strainV h1V //(h1V*h1V*h1V*h1V*h1V)
#define h2strainV h2V //(h2V*h2V*h2V*h2V*h2V)
#define h3strainV h3V //(h3V*h3V*h3V*h3V*h3V)
#define hn1strainV hn1V //(5.0*h1V*h1V*h1V*h1V*hn1V)
#define hn2strainV hn2V //(5.0*h2V*h2V*h2V*h2V*hn2V)
#define hn3strainV hn3V //(5.0*h3V*h3V*h3V*h3V*hn3V)

// This double-well function can be used to tune the interfacial energy
#define fbarrierV (n1*n1-2.0*n1*n1*n1+n1*n1*n1*n1)
#define fbarriernV (2.0*n1-6.0*n1*n1+4.0*n1*n1*n1)

// Residuals
#define rcV   (c)
#define rcxTemp ( cx*((1.0-h1V-h2V-h3V)*faccV+(h1V+h2V+h3V)*fbccV) + n1x*((fbcV-facV)*hn1V) + n2x*((fbcV-facV)*hn2V) + n3x*((fbcV-facV)*hn3V) )
#define rcxV  (constV(-timeStep*McV)*rcxTemp)

#define rn1V   (n1-constV(timeStep*Mn1V)*((fbV-faV)*hn1V+W*fbarriernV+nDependentMisfitAC1))
#define rn2V   (n2-constV(timeStep*Mn2V)*((fbV-faV)*hn2V))
#define rn3V   (n3-constV(timeStep*Mn3V)*((fbV-faV)*hn3V))
#define rn1xV  (constV(-timeStep*Mn1V)*Knx1)
#define rn2xV  (constV(-timeStep*Mn2V)*Knx2)
#define rn3xV  (constV(-timeStep*Mn3V)*Knx3)

// Initial geometry
#define x_denom 1.0
#define y_denom 1.0
#define z_denom 1.0
#define initial_interface_coeff 1.0
#define initial_radius 6.0
#define avg_Nd 0.004
#define c_matrix 0.004
