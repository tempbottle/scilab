/*
 *  Scilab ( http://www.scilab.org/ ) - This file is part of Scilab
 *  Copyright (C) 2008-2008 - DIGITEO - Antoine ELIAS
 *
 *  This file must be used under the terms of the CeCILL.
 *  This source file is licensed as described in the file COPYING, which
 *  you should have received as part of this distribution.  The terms
 *  are also available at
 *  http://www.cecill.info/licences/Licence_CeCILL_V2-en.txt
 *
 */

// This code is separated in run_AssignExp.hxx
// but will be inlined in runvisitor.hxx
// using #include with RunVisitorT class declaration.
//
// If you need additionnal headers, please add it in runvisitor.hxx

void visitprivate(const AssignExp  &e)
{
    /*Create local exec visitor*/
    try
    {
        /*get king of left hand*/
        const SimpleVar *pVar = dynamic_cast<const SimpleVar*>(&e.left_exp_get());
        if(pVar)
        {// x = ?
            /*getting what to assign*/
            expected_size_set(1);
            e.right_exp_get().accept(*this);

            if(result_getSize() != 1)
            {
                std::wostringstream os;
                os << L"Can not assign multiple value in a single variable" << std::endl;;
                //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            InternalType *pIT = result_get();

            //reset result
            result_set(NULL);
            if(pIT->isImplicitList())
            {
                if(pIT->getAs<ImplicitList>()->isComputable())
                {
                    InternalType *pTemp = pIT->getAs<ImplicitList>()->extractFullMatrix();
                    delete pIT;
                    pIT = pTemp;
                }
            }

            const ReturnExp *pReturn = dynamic_cast<const ReturnExp*>(&e.right_exp_get());
            if(pReturn)
            {//ReturnExp so, put the value in the previous scope
                symbol::Context::getInstance()->put_in_previous_scope(pVar->name_get(), *pIT);
                ((AssignExp*)&e)->break_set();
            }
            else
            {
                symbol::Context::getInstance()->put(pVar->name_get(), *pIT);
            }

            if(e.is_verbose() && ConfigVariable::isPromptShow())
            {
                std::wostringstream ostr;
                ostr << pVar->name_get().name_get() << L"  = " << std::endl << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pIT);
            }
            return;
        }

        const CellCallExp *pCell = dynamic_cast<const CellCallExp*>(&e.left_exp_get());
        if(pCell)
        {
            InternalType *pIT;
            bool bRet           = true;
            bool bNew           = false;

            //retrieve variable
            pVar = dynamic_cast<const SimpleVar*>(&pCell->name_get());
            if(pVar == NULL)
            {//manage a.b{1} = x
                pCell->name_get().accept(*this);

                if(result_get() != NULL && result_get()->isCell())
                {
                    pIT = result_get();
                }
                else
                {//never append ?
                    std::wostringstream os;
                    os << _W("Unable to extract left part expression.\n");
                    //os << ((Location)e.left_exp_get().location_get()).location_getString() << std::endl;
                    throw ScilabError(os.str(), 999, e.left_exp_get().location_get());
                }
                //reset result
                result_set(NULL);
            }
            else
            {
                pIT = symbol::Context::getInstance()->get(pVar->name_get());
            }

            /*getting what to assign*/
            e.right_exp_get().accept(*this);
            InternalType* pITR = result_get();

            if(pITR == NULL)
            {// if the right hand is NULL.
                std::wostringstream os;
                os << _W("Unable to extract right part expression.\n");
                throw ScilabError(os.str(), 999, e.left_exp_get().location_get());
            }

            //reset result
            result_set(NULL);

            if(pIT == NULL)
            {//Var doesn't exist, create it with good dimensions
                bNew = true;
            }
            else
            {
                if(pIT->isRef(1) == true)
                {
                    pIT = pIT->clone();
                    bNew = true;
                }
            }

            InternalType *pOut	= NULL;
            typed_list *pArgs = GetArgumentList(pCell->args_get());

            //fisrt extract implicit list
            if(pITR->isImplicitList())
            {
                InternalType *pIL = ((InternalType*)result_get())->getAs<ImplicitList>()->extractFullMatrix();
                if(result_get()->isDeletable())
                {
                    delete result_get();
                }
                result_set(pIL);
            }
            else if(result_get()->isContainer() && result_get()->isDeletable() == false)
            {
                InternalType* pIL = result_get()->clone();
                result_set(pIL);
            }


            if(pIT == NULL)
            {//call static insert function
                pOut = Cell::insertNewCell(pArgs, result_get());
            }
            else
            {//call type insert function
                pOut = pIT->getAs<Cell>()->insertCell(pArgs, result_get());

                if(pOut && pOut != pIT)
                {
                    //variable change
                    pIT->DecreaseRef();
                    if(pIT->isDeletable())
                    {
                        delete pIT;
                    }
                    bNew = true;
                }
            }


            if(pOut != NULL)
            {
                if(bNew)
                {
                    symbol::Context::getInstance()->put(pVar->name_get(), *pOut);
                }

                if(e.is_verbose() && ConfigVariable::isPromptShow())
                {
                    std::wostringstream ostr;
                    if(pVar)
                    {
                        ostr << pVar->name_get().name_get() << L"  = " << std::endl;
                    }
                    else
                    {
                        ostr << L"???" << L"  = " << std::endl;
                    }
                    ostr << std::endl;
                    VariableToString(pOut);
                }
            }
            else
            {
                //manage error
                std::wostringstream os;
                os << _W("Invalid Index.\n");
                //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }
//            delete piMaxDim;
//            delete[] piDimSize;
            for(int iArg = 0 ; iArg < pArgs->size() ; iArg++)
            {
                if((*pArgs)[iArg]->isDeletable())
                {
                    delete (*pArgs)[iArg];
                }
            }
            delete pArgs;
            return;
        }

        const CallExp *pCall = dynamic_cast<const CallExp*>(&e.left_exp_get());
        if(pCall)
        {//x(?) = ?
            InternalType *pIT;
            bool bNew   = false;

            pVar = dynamic_cast<const SimpleVar*>(&pCall->name_get());
            if(pVar == NULL)
            {//manage a.b(1) = x
                pCall->name_get().accept(*this);

                if(result_get() != NULL)
                {
                    pIT = result_get();
                }
                else
                {//never append ?
                    std::wostringstream os;
                    os << _W("Unable to extract left part expression.\n");
                    //os << ((Location)e.left_exp_get().location_get()).location_getString() << std::endl;
                    throw ScilabError(os.str(), 999, e.left_exp_get().location_get());
                }

                result_set(NULL);
            }
            else
            {
                pIT = symbol::Context::getInstance()->get(pVar->name_get());
            }

            /*getting what to assign*/
            e.right_exp_get().accept(*this);
            InternalType* pITR = result_get();

            if(pITR == NULL)
            {// if the right hand is NULL.
                std::wostringstream os;
                os << _W("Unable to extract right part expression.\n");
                throw ScilabError(os.str(), 999, e.left_exp_get().location_get());
            }

            //reset result
            result_set(NULL);

            if(pIT == NULL)
            {//Var doesn't exist, create it with good dimensions
                bNew = true;
            }
            else
            {
                if(pIT->isRef(1) == true)
                {
                    InternalType* pITTemp = pIT->clone();
                    pIT = pITTemp;
                    bNew = true;
                }
            }

            typed_list *pArgs = GetArgumentList(pCall->args_get());
            InternalType *pOut	= NULL;

            //fisrt extract implicit list
            if(pITR->isImplicitList())
            {
                InternalType *pIL = pITR->getAs<ImplicitList>()->extractFullMatrix();
                pITR = pIL;
            }
            else if(pITR->isContainer() && pITR->isRef())
            {
                //std::cout << "assign container type during insertion" << std::endl;
                InternalType* pIL = pITR->clone();
                pITR = pIL;
            }

            if(pITR->isDouble() && pITR->getAs<Double>()->isEmpty() && pIT == NULL)
            {
                // l(x) = [] when l is not defined => create l = []
                pOut = Double::Empty();
                bNew = true;
            }
            else if(pITR->isDouble() && pITR->getAs<Double>()->isEmpty() && pIT->isStruct() == false && pIT->isList() == false)
            {//insert [] so deletion except for Struct and List which can insert []
                if(pIT->isDouble())
                {
                    pOut = pIT->getAs<Double>()->remove(pArgs);
                }
                else if(pIT->isString())
                {
                    pOut = pIT->getAs<String>()->remove(pArgs);
                }
                else if(pIT->isCell())
                {
                    pOut = pIT->getAs<Cell>()->remove(pArgs);
                }
                else if(pIT->isBool())
                {
                    pOut = pIT->getAs<Bool>()->remove(pArgs);
                }
                else if(pIT->isPoly())
                {
                    pOut = pIT->getAs<Polynom>()->remove(pArgs);
                }
                else if(pIT->isInt8())
                {
                    pOut = pIT->getAs<Int8>()->remove(pArgs);
                }
                else if(pIT->isUInt8())
                {
                    pOut = pIT->getAs<UInt8>()->remove(pArgs);
                }
                else if(pIT->isInt16())
                {
                    pOut = pIT->getAs<Int16>()->remove(pArgs);
                }
                else if(pIT->isUInt16())
                {
                    pOut = pIT->getAs<UInt16>()->remove(pArgs);
                }
                else if(pIT->isInt32())
                {
                    pOut = pIT->getAs<Int32>()->remove(pArgs);
                }
                else if(pIT->isUInt32())
                {
                    pOut = pIT->getAs<UInt32>()->remove(pArgs);
                }
                else if(pIT->isInt64())
                {
                    pOut = pIT->getAs<Int64>()->remove(pArgs);
                }
                else if(pIT->isUInt64())
                {
                    pOut = pIT->getAs<UInt64>()->remove(pArgs);
                }
                else if(pIT->isStruct())
                {
                    // a("b") = [] is not a deletion !!
                    Struct* pStr = pIT->getAs<Struct>();

                    pOut = pIT->getAs<Struct>()->insert(pArgs, pITR);
                }

                if(pOut && pOut != pIT)
                {
                    bNew = true;
                }
            }
            else if(pIT == NULL || (pIT->isDouble() && pIT->getAs<Double>()->getSize() == 0))
            {
                //insert in a new variable or []
                //call static insert function
                //special case for insertion in []
                if(pIT != NULL && pIT->isDouble() && pIT->getAs<Double>()->getSize() == 0)
                {
                    bNew = true;
                }

                //if pIT == NULL and pArg is single string, it's a struct creation
                if((*pArgs)[0]->isString())
                {
                    String *pS = (*pArgs)[0]->getAs<types::String>();
                    Struct* pStr = new Struct(1, 1);

                    if(pArgs->size() != 1 || pS->isScalar() == false)
                    {
                        //manage error
                        std::wostringstream os;
                        os << _W("Invalid Index.\n");
                        //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
                        throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
                    }

                    pStr->addField(pS->get(0));
                    pStr->get(0)->set(pS->get(0), pITR);
                    pOut = pStr;
                }
                else
                {
                    switch(pITR->getType())
                    {
                    case InternalType::RealDouble :
                        pOut = Double::insertNew(pArgs, pITR);
                        break;
                    case InternalType::RealString :
                        pOut = String::insertNew(pArgs, pITR);
                        break;
                    case InternalType::RealCell :
                        pOut = Cell::insertNew(pArgs, pITR);
                        break;
                    case InternalType::RealBool :
                        pOut = Bool::insertNew(pArgs, pITR);
                        break;
                    case InternalType::RealPoly :
                        pOut = Polynom::insertNew(pArgs, pITR);
                        break;
                    case InternalType::RealInt8 :
                        pOut = Int8::insertNew(pArgs, pITR);
                        break;
                    case InternalType::RealUInt8 :
                        pOut = UInt8::insertNew(pArgs, pITR);
                        break;
                    case InternalType::RealInt16 :
                        pOut = Int16::insertNew(pArgs, pITR);
                        break;
                    case InternalType::RealUInt16 :
                        pOut = UInt16::insertNew(pArgs, pITR);
                        break;
                    case InternalType::RealInt32 :
                        pOut = Int32::insertNew(pArgs, pITR);
                        break;
                    case InternalType::RealUInt32 :
                        pOut = UInt32::insertNew(pArgs, pITR);
                        break;
                    case InternalType::RealInt64 :
                        pOut = Int64::insertNew(pArgs, pITR);
                        break;
                    case InternalType::RealUInt64 :
                        pOut = UInt64::insertNew(pArgs, pITR);
                        break;
                    default :
                        {
                            //manage error
                            std::wostringstream os;
                            os << _W("Operation not yet managed.\n");
                            //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
                            throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
                            break;
                        }
                    }
                }
            }
            else
            {//call type insert function
                InternalType* pRet = NULL;
                InternalType* pInsert = pITR;
                //check types compatibilties

                if(pIT->isDouble() && pInsert->isDouble())
                {
                    pRet = pIT->getAs<Double>()->insert(pArgs, pInsert);
                }
                else if(pIT->isString() && pInsert->isString())
                {
                    pRet = pIT->getAs<String>()->insert(pArgs, pInsert);
                }
                else if(pIT->isCell() && pInsert->isCell())
                {
                    pRet = pIT->getAs<Cell>()->insert(pArgs, pInsert);
                }
                else if(pIT->isBool() && pInsert->isBool())
                {
                    pRet = pIT->getAs<Bool>()->insert(pArgs, pInsert);
                }
                else if(pIT->isSparse() && pInsert->isDouble())
                {
                    pRet = pIT->getAs<Sparse>()->insert(pArgs, pInsert);
                }
                else if(pIT->isPoly() && pInsert->isPoly())
                {
                    pRet = pIT->getAs<Polynom>()->insert(pArgs, pInsert);
                }
                else if(pIT->isInt8() && pInsert->isInt8())
                {
                    pRet = pIT->getAs<Int8>()->insert(pArgs, pInsert);
                }
                else if(pIT->isUInt8() && pInsert->isUInt8())
                {
                    pRet = pIT->getAs<UInt8>()->insert(pArgs, pInsert);
                }
                else if(pIT->isInt16() && pInsert->isInt16())
                {
                    pRet = pIT->getAs<Int16>()->insert(pArgs, pInsert);
                }
                else if(pIT->isUInt16() && pInsert->isUInt16())
                {
                    pRet = pIT->getAs<UInt16>()->insert(pArgs, pInsert);
                }
                else if(pIT->isInt32() && pInsert->isInt32())
                {
                    pRet = pIT->getAs<Int32>()->insert(pArgs, pInsert);
                }
                else if(pIT->isUInt32() && pInsert->isUInt32())
                {
                    pRet = pIT->getAs<UInt32>()->insert(pArgs, pInsert);
                }
                else if(pIT->isInt64() && pInsert->isInt64())
                {
                    pRet = pIT->getAs<Int64>()->insert(pArgs, pInsert);
                }
                else if(pIT->isUInt64() && pInsert->isUInt64())
                {
                    pRet = pIT->getAs<UInt64>()->insert(pArgs, pInsert);
                }
                else if(pIT->isList())
                {
                    pRet = pIT->getAs<List>()->insert(pArgs, pInsert);
                }
                else if(pIT->isStruct())
                {
                    Struct* pStr = pIT->getAs<Struct>();
                    if(pArgs->size() == 1 && (*pArgs)[0]->isString())
                    {//s("x") = y
                        String *pS = (*pArgs)[0]->getAs<types::String>();
                        if(pS->isScalar() == false)
                        {
                            //manage error
                            std::wostringstream os;
                            os << _W("Invalid Index.\n");
                            //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
                            throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
                        }

                        pStr->addField(pS->get(0));
                        pStr->get(0)->set(pS->get(0), pInsert);
                        pRet = pStr;
                    }
                    else
                    {
                            pRet = pStr->insert(pArgs, pInsert);
                    }
                }
                else
                {//overloading
                    types::typed_list in;
                    types::typed_list out;

                    //overload insertion
                    //%x_i_x(i1, i2, ..., in, origin, inserted)
                    //i1, ..., in : indexes
                    //origin : variable where to insert data
                    //inserted : data to insert

                    for(int i = 0 ; i < pArgs->size() ; i++)
                    {
                        (*pArgs)[i]->IncreaseRef();
                        in.push_back((*pArgs)[i]);
                    }

                    pInsert->IncreaseRef();
                    in.push_back(pInsert);

                    pIT->IncreaseRef();
                    in.push_back(pIT);

                    //build function name
                    //a_i_b
                    //a : type to insert
                    //b : type that receive data
                    std::wstring function_name;
                    function_name = L"%" + pInsert->getShortTypeStr() + L"_i_" + pIT->getShortTypeStr();
                    Overload::call(function_name, in, 1, out, this);

                    pIT->DecreaseRef();
                    pInsert->DecreaseRef();
                    for(int i = 0 ; i < pArgs->size() ; i++)
                    {
                        (*pArgs)[i]->DecreaseRef();
                    }

                    if(out.size() != 0)
                    {
                        pRet = out[0];
                    }
                    else
                    {
                        pRet = NULL;
                    }
                }

                if(pRet && pRet != pIT)
                {
                    bNew = true;
                }

                pOut = pRet;
            }

            if(pOut != NULL)
            {
                if(bNew)
                {
                    if(pVar == NULL)
                    {
                        //is not a(x) = y but something like a.b(x) = y
                        //so we have to retrieve struct and children to assign new value
                        InternalType *pHead = NULL;
                        Struct* pMain       = NULL;
                        Struct* pCurrent    = NULL;
                        bool bOK = getStructFromExp(&pCall->name_get(), &pMain, &pCurrent, NULL, pOut);
                        //change pOut only to toString call
                        pOut = pMain;
                    }
                    else
                    {
                        symbol::Context::getInstance()->put(pVar->name_get(), *((GenericType*)pOut));
                    }
                }

                if(e.is_verbose() && ConfigVariable::isPromptShow())
                {
                    std::wostringstream ostr;
                    if(pVar)
                    {
                        ostr << pVar->name_get().name_get() << L"  = " << std::endl;
                    }
                    else
                    {
                        ostr << *getStructNameFromExp(&pCall->name_get()) << L"  = " << std::endl;
                    }

                    ostr << std::endl;
                    scilabWriteW(ostr.str().c_str());
                    VariableToString(pOut);
                }
            }
            else
            {
                //manage error
                std::wostringstream os;
                os << _W("Invalid index.\n");
                //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }
            //delete piMaxDim;
            //delete[] piDimSize;
            for(int iArg = 0 ; iArg < pArgs->size() ; iArg++)
            {
                if((*pArgs)[iArg]->isDeletable())
                {
                    delete (*pArgs)[iArg];
                }
            }
            result_clear();
            delete pArgs;
            return;
        }

        const AssignListExp *pList = dynamic_cast<const AssignListExp*>(&e.left_exp_get());
        if(pList)
        {//[x,y] = ?
            int iLhsCount = (int)pList->exps_get().size();

            /*getting what to assign*/
            expected_size_set(iLhsCount);
            e.right_exp_get().accept(*this);

            if(result_getSize() != iLhsCount)
            {
                std::wostringstream os;
                os << L"Incompatible assignation: trying to assign " << result_getSize();
                os << " values in " << iLhsCount << " variables." << std::endl;
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }


            std::list<Exp *>::const_reverse_iterator it;
            int i = (int)iLhsCount - 1;
            for(it = pList->exps_get().rbegin() ; it != pList->exps_get().rend() ; it++,i--)
            {
                const SimpleVar *pListVar	= dynamic_cast<const SimpleVar*>((*it));
                symbol::Context::getInstance()->put(pListVar->name_get(), *result_get(i));
                if(e.is_verbose() && ConfigVariable::isPromptShow())
                {
                    std::wostringstream ostr;
                    ostr << pListVar->name_get().name_get() << L"  = " << std::endl;
                    ostr << std::endl;
                    scilabWriteW(ostr.str().c_str());
                    VariableToString(result_get(i));
                }
                //clear result to take care of [n,n]
                result_set(i, NULL);
            }
            result_clear();
            return;
        }
        
        const FieldExp *pField = dynamic_cast<const FieldExp*>(&e.left_exp_get());
        if(pField)
        {//a.b = x
            //a.b can be a struct or a tlist/mlist
            InternalType *pHead = NULL;
            Struct* pMain       = NULL;
            Struct* pCurrent    = NULL;

            /*getting what to assign*/
            expected_size_set(1);
            e.right_exp_get().accept(*this);

            bool bOK = getStructFromExp(pField, &pMain, &pCurrent, NULL, result_get());
            if(pMain != NULL)
            {
                pHead = pMain;
            }
            else
            {
                //a is not a struct
                const SimpleVar* pListVar =  dynamic_cast<const SimpleVar*>(pField->head_get());
                if(pListVar == NULL)
                {
                    std::cout << "Houston ..." << std::endl;
                }
                pHead = symbol::Context::getInstance()->get(pListVar->name_get());
            }

            //if a is already assign, make a copy and replace it
            if(pHead->isRef(1) == true)
            {
                pHead = pHead->clone();
                const wstring *pstName = getStructNameFromExp(pField);
                symbol::Context::getInstance()->put(symbol::Symbol(*pstName), *pHead);
            }

            //we can assign only one value
            if(result_getSize() != 1)
            {
                std::wostringstream os;
                os << L"Lhs != Rhs";
                //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
                throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
            }

            InternalType *pIT = result_get();
            if(pIT->isImplicitList())
            {
                if(pIT->getAs<ImplicitList>()->isComputable())
                {
                    InternalType *pTemp = pIT->getAs<ImplicitList>()->extractFullMatrix();
                    delete pIT;
                    result_set(NULL);
                    pIT = pTemp;
                }
            }
            else if(pIT->isContainer())
            {//if assigned value is a container, copy it before assign.
                //std::cout << "assign container type to field" << std::endl;
                //pIT = pIT->clone();
            }

            //assign result to new field
            const SimpleVar* pTail =  dynamic_cast<const SimpleVar*>(pField->tail_get());

            if(pHead->isStruct())
            {
                //@STRUCT
                //fillStructFromExp(pField->tail_get(), pStr, 0, pIT);
            }
            else if(pHead->isTList())
            {
                TList* pT = pHead->getAs<TList>();
                if(pT->exists(pTail->name_get().name_get()))
                {
                    pT->set(pTail->name_get().name_get(), pIT);
                }
                else
                {
                    std::wostringstream os;
                    os << L"Field must be exist";
                    throw ScilabError(os.str(), 999, pVar->location_get());
                }
            }
            else if(pHead->isMList())
            {
                //TODO:
            }

            if(e.is_verbose() && ConfigVariable::isPromptShow())
            {
                const wstring *pstName = getStructNameFromExp(pField);

                types::InternalType* pPrint = symbol::Context::getInstance()->get(symbol::Symbol(*pstName));
                std::wostringstream ostr;
                ostr << *pstName << L"  = " << std::endl << std::endl;
                scilabWriteW(ostr.str().c_str());
                VariableToString(pPrint);
            }

            result_clear();
            return;
        }

        std::wostringstream os;
        os << L"unknow script form";
        //os << ((Location)e.right_exp_get().location_get()).location_getString() << std::endl;
        throw ScilabError(os.str(), 999, e.right_exp_get().location_get());
    }
    catch(ScilabError error)
    {
        throw error;
    }
}