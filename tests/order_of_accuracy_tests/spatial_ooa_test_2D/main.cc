// Beta prime precipitate evolution implementation
// Code to calculate the steady-state morphology of a single precipitate
//general headers
#include "../../../include/dealIIheaders.h"

//Coupled Cahn-Hilliard+Allen-Cahn+Mechanics problem headers
//#include "parameters_bPPE.h"
#include "space_parameters.h"
#include "parameters.h"
#include "../../../src/models/coupled/coupledCHACMechanics.h"

//initial condition for concentration
template <int dim>
class InitialConditionC : public Function<dim>
{
public:
  InitialConditionC () : Function<dim>(1) {
    std::srand(Utilities::MPI::this_mpi_process(MPI_COMM_WORLD)+1);
  }
  double value (const Point<dim> &p, const unsigned int component = 0) const
  {
    //return the value of the initial concentration field at point p
	double dx=spanX/( (double)subdivisionsX )/std::pow(2.0,refineFactor);
	double dy=spanY/( (double)subdivisionsY )/std::pow(2.0,refineFactor);
	double dz=spanZ/( (double)subdivisionsZ )/std::pow(2.0,refineFactor);
	double r = 0.0;
    //return 0.02 + 1.0e-3*(2*(0.5 - (double)(std::rand() % 100 )/100.0));
	#if problemDIM==1
	  r=p.operator()(0);
	  return 0.5*(0.12-0.00)*(1-std::tanh((r-spanX/16.0)/(0.1*dx)));
	#elif problemDIM==2

	  r=sqrt((p.operator()(0))*(p.operator()(0))/x_denom
	  		+(p.operator()(1))*(p.operator()(1))/y_denom);
	  return 0.5*(0.12-avg_Nd)*(1.0-std::tanh((r-initial_radius)/(initial_interface_coeff))) + avg_Nd;

	#elif problemDIM==3

	  r=sqrt((p.operator()(0))*(p.operator()(0))/x_denom
		+(p.operator()(1))*(p.operator()(1))/y_denom
		+(p.operator()(2))*(p.operator()(2))/z_denom);
	  return 0.5*(0.12-avg_Nd)*(1.0-std::tanh((r-initial_radius)/(initial_interface_coeff))) + avg_Nd;


	#endif
  }
};

//initial condition for the structural order parameters
template <int dim>
class InitialConditionN : public Function<dim>
{
public:
  unsigned int index;
  InitialConditionN (const unsigned int _index) : Function<dim>(1), index(_index) {
    std::srand(Utilities::MPI::this_mpi_process(MPI_COMM_WORLD)+1);
  }
  double value (const Point<dim> &p, const unsigned int component = 0) const
  {
    //set result equal to the structural order paramter initial condition
	  double dx=spanX/( (double)subdivisionsX )/std::pow(2.0,refineFactor);
	  double dy=spanY/( (double)subdivisionsY )/std::pow(2.0,refineFactor);
	  double dz=spanZ/( (double)subdivisionsZ )/std::pow(2.0,refineFactor);
    double r=0.0;
	#if problemDIM==1
    r=p.operator()(0);
	  return 0.5*(1.0-std::tanh((r-spanX/16.0)/(0.1*dx)));
	#elif problemDIM==2
	  if (index==1){
		  r=sqrt((p.operator()(0))*(p.operator()(0))/x_denom
		  		  		+(p.operator()(1))*(p.operator()(1))/y_denom);
		  return 0.5*(1.0-std::tanh((r-initial_radius)/(initial_interface_coeff)));
	  }
	  else if (index==2){
		return 0.0;
	//     double r1=p.distance(Point<dim>(3*spanX/4.0,spanY/4.0));
	//     double r2=p.distance(Point<dim>(spanX/2.0,spanY/2.0));
	//     r=std::min(r1,r2);
	  }
	  else if (index==3){
		return 0.0;
	//     r=p.distance(Point<dim>(spanX/4.0,3*spanY/4.0));
	  }
	  //return 0.5*(1.0-std::tanh((r-spanX/16.0)/(3*dx)));
	#elif problemDIM==3
	  if (index==1){
	  //r=p.distance(Point<dim>(spanX/2.0,spanY/2.0,spanZ/2.0));
	  //return 0.5*(1.0-std::tanh((r-spanX/8.0)/(3*dx)));

		r=sqrt((p.operator()(0))*(p.operator()(0))/x_denom
		  		+(p.operator()(1))*(p.operator()(1))/y_denom
		  		+(p.operator()(2))*(p.operator()(2))/z_denom);
		return 0.5*(1.0-std::tanh((r-initial_radius)/(initial_interface_coeff)));

		// planar interface
		//r=sqrt((p.operator()(2)-spanZ/2.0)*(p.operator()(2)-spanZ/2.0));
		//return 0.5*(1.0-std::tanh((r)/(initial_interface_coeff)));
		//return 0.5*(1.0-std::tanh((r-initial_radius)/(initial_interface_coeff)));
	  }
	  else if (index==2){
		return 0.0;
	  }
	  else if (index==3){
		return 0.0;
	  }
	#endif
	return 0.0;
  }
};


//apply initial conditions
template <int dim>
void CoupledCHACMechanicsProblem<dim>::applyInitialConditions()
{
  unsigned int fieldIndex;
  //call initial condition function for c
  fieldIndex=this->getFieldIndex("c");
  VectorTools::interpolate (*this->dofHandlersSet[fieldIndex], InitialConditionC<dim>(), *this->solutionSet[fieldIndex]);
  //call initial condition function for structural order parameters
  fieldIndex=this->getFieldIndex("n1");
  VectorTools::interpolate (*this->dofHandlersSet[fieldIndex], InitialConditionN<dim>(1), *this->solutionSet[fieldIndex]);
  fieldIndex=this->getFieldIndex("n2");
  VectorTools::interpolate (*this->dofHandlersSet[fieldIndex], InitialConditionN<dim>(2), *this->solutionSet[fieldIndex]);
  fieldIndex=this->getFieldIndex("n3");
  VectorTools::interpolate (*this->dofHandlersSet[fieldIndex], InitialConditionN<dim>(3), *this->solutionSet[fieldIndex]);
  //set zero intial condition for u
  fieldIndex=this->getFieldIndex("u");
  *this->solutionSet[fieldIndex]=0.0;
}

//apply Dirchlet BC function
template <int dim>
void CoupledCHACMechanicsProblem<dim>::applyDirichletBCs(){
  //Set u=0 at all boundaries
  VectorTools::interpolate_boundary_values (*this->dofHandlersSet[this->getFieldIndex("u")],\
					    0, ZeroFunction<dim>(dim), *(ConstraintMatrix*) \
					    this->constraintsSet[this->getFieldIndex("u")]);
}

//methods to mark boundaries
template <int dim>
void CoupledCHACMechanicsProblem<dim>::markBoundaries(){
	// All boundaries are marked zero by default
}

//main
int main (int argc, char **argv)
{
  Utilities::MPI::MPI_InitFinalize mpi_initialization(argc, argv,numbers::invalid_unsigned_int);
  try
    {
      deallog.depth_console(0);
      CoupledCHACMechanicsProblem<problemDIM> problem;
      problem.fields.push_back(Field<problemDIM>(SCALAR, PARABOLIC, "c"));
      problem.fields.push_back(Field<problemDIM>(SCALAR, PARABOLIC, "n1"));
      problem.fields.push_back(Field<problemDIM>(SCALAR, PARABOLIC, "n2"));
      problem.fields.push_back(Field<problemDIM>(SCALAR, PARABOLIC, "n3"));
      problem.fields.push_back(Field<problemDIM>(VECTOR,  ELLIPTIC, "u"));
      problem.init ();
      problem.solve();
    }
  catch (std::exception &exc)
    {
      std::cerr << std::endl << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Exception on processing: " << std::endl
                << exc.what() << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;
      return 1;
    }
  catch (...)
    {
      std::cerr << std::endl << std::endl
                << "----------------------------------------------------"
                << std::endl;
      std::cerr << "Unknown exception!" << std::endl
                << "Aborting!" << std::endl
                << "----------------------------------------------------"
                << std::endl;
      return 1;
    }

  return 0;
}



