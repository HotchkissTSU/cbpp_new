#ifndef CBPP_ENTITY_BASE_H
#define CBPP_ENTITY_BASE_H

#include "cbpp/Array.h"
#include "cbpp/String.h"
#include "cbpp/Constants.h"

#include "engine/ent_utils.h"

namespace cbpp::ent {

    // A basis for all game entities
    class CBase {
        public: 
            virtual const char* Classname() { return "CBase"; }                                                                 
            virtual const char* Base() { return "CB++"; }    

            class Datadesc : public cbpp::IEntityDatadesc {
                protected: 
                    CArray<IEntityProperty*> m_aProps;
                    Datadesc* __get_this(); 
                public:                                   
                    virtual ~Datadesc();

                    size_t Length();
                    IEntityProperty* At(size_t iIndex);
                    void PushProperty(IEntityProperty* pProp);

                    CbStringPropertyEx(sWorldName, "unnamed", EStringType::EntityName)
            };

            CBase() = delete;
            CBase(Datadesc* pData) { this->Init(pData); }                                                                    
            virtual ~CBase();  

        private: 
            static IEntityDatadesc* CreateDatadesc() { return (cbpp::IEntityDatadesc*)(cbpp::New<Datadesc>()); }              
            inline static cbpp::CEntityRegistrator __s_registrator = cbpp::CEntityRegistrator( "CBase", NULL, CBase::CreateDatadesc );

        public:
            void Init(Datadesc*);
            void Destruct();

    };
}

#endif
