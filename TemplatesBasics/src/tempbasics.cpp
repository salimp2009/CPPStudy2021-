#include "Templpch.h"

#include "TemplateBasicsExamples.hpp"
#include "VariadicTemplates.hpp"
#include "TrickyBasicsExamples.hpp"
#include "Concepts/ConceptExamples.hpp"
#include "Concepts/ConceptExamples2.hpp"
#include "Concepts/ConceptExamples3.hpp"
#include "EmptyBaseClassOptm/EmptyBaseClassOptTests.hpp"
#include "MetaProgramming/MetaProgrammingBasics.hpp"

int main()
{
	//FunctionTemplates_Basics();
	//NonTemplateParams_Basics();
	VariadicTemplate_Basics(); 
	//FoldingBinaryTree();
	VariadicIndices();
	//VariadicBases_Overloader();
	//TrickyBasics01();
	//TrickyBasics_Arrays();
	//ArrayOverloads_Test();
	//MemberFunctSpecialization_Test();
	//VariableTemplates_Test();
	//greatCommonDiv_Concepts();
	//compileTimePredicate_Concepts();
	//customConceptExample1_();
	//ConceptsIn_VariadicTemplates();
	//TestingCustomConcept();
	//FunctionOverloadingConcepts();
	//SpecializeTemplates_Concepts();
	conceptBasedDispatch();
	//placeHolders_Concepts();
	//overloadWithConcepts();
	//dataValidateCheck_Concepts();
	//optionalImplemt_Concepts();
	//SubSumptionRules_Concepts();
	//PrintContainers_Concepts();
	//SignedUnSignedIntegral_Func();
	//RegularSemiRegular_Concepts();
	EmptyBaseClassTest1();
	metaprog_TypeManipulation();
	metaprog_MetaFunctvsRunTime();
	return 0;
}