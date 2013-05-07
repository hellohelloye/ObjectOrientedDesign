/////////////////////////////////////////////////////////////////////
//  FoldingRules.cpp - declares ITokCollection folding rules       //
//  ver 1.0                                                        //
//  Language:       Visual C++, ver 6.0                            //
//  Platform:       MacBook Pro, Windows7 Pro				       //
//  Application:    CSE687 project #3	                           //
//  Author:         YUKUI YE, Syracuse University	     		   //
//                  (315) 751-0156, mollysmile.ye@gmail.com        //
/////////////////////////////////////////////////////////////////////

#include "FoldingRules.h"

void codeFoldingRules::MergeForCollections(ITokCollection*& pTc)
{
  // have for(int i=0;
  pTc->get(false);  // get i<N;
  pTc->get(false);  // get ++i) {
}

void codeFoldingRules::MergeOperatorTokens(ITokCollection*& pTc)
{
  pTc->merge("operator","(");
}

void codeFoldingRules::doFold(ITokCollection*& pTc)
{
  if(pTc->find("for") < pTc->length())
    codeFoldingRules::MergeForCollections(pTc);
  if(pTc->find("operator") < pTc->length())
    codeFoldingRules::MergeOperatorTokens(pTc);
}