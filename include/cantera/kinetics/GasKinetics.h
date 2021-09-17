/**
 * @file GasKinetics.h
 * @ingroup chemkinetics
 */

// This file is part of Cantera. See License.txt in the top-level directory or
// at https://cantera.org/license.txt for license and copyright information.

#ifndef CT_GASKINETICS_H
#define CT_GASKINETICS_H

#include "BulkKinetics.h"
#include "FalloffMgr.h"
#include "Reaction.h"

namespace Cantera
{

/**
 * Kinetics manager for elementary gas-phase chemistry. This kinetics manager
 * implements standard mass-action reaction rate expressions for low-density
 * gases.
 * @ingroup kinetics
 */
class GasKinetics : public BulkKinetics
{
public:
    //! @name Constructors and General Information
    //! @{

    //! Constructor.
    /*!
     *  @param thermo  Pointer to the gas ThermoPhase (optional)
     */
    GasKinetics(ThermoPhase* thermo = 0);

    virtual std::string kineticsType() const {
        return "Gas";
    }

    //! @}
    //! @name Reaction Rates Of Progress
    //! @{

    virtual void getEquilibriumConstants(doublereal* kc);
    virtual void getFwdRateConstants(double* kfwd);

    //! @}
    //! @name Reaction Mechanism Setup Routines
    //! @{
    virtual void init();
    virtual bool addReaction(shared_ptr<Reaction> r, bool resize=true);
    virtual void modifyReaction(size_t i, shared_ptr<Reaction> rNew);
    virtual void invalidateCache();
    //@}

    void updateROP();

    //! Update temperature-dependent portions of reaction rates and falloff
    //! functions.
    virtual void update_rates_T();

    //! Update properties that depend on concentrations.
    //! Currently the enhanced collision partner concentrations are updated
    //! here, as well as the pressure-dependent portion of P-log and Chebyshev
    //! reactions.
    virtual void update_rates_C();

protected:
    //! Reaction index of each falloff reaction
    std::vector<size_t> m_fallindx;

    //! Map of reaction index to falloff reaction index (i.e indices in
    //! #m_falloff_low_rates and #m_falloff_high_rates)
    std::map<size_t, size_t> m_rfallindx;

    //! Rate expressions for falloff reactions at the low-pressure limit
    Rate1<Arrhenius> m_falloff_low_rates;

    //! Rate expressions for falloff reactions at the high-pressure limit
    Rate1<Arrhenius> m_falloff_high_rates;

    FalloffMgr m_falloffn;

    ThirdBodyCalc m_3b_concm;
    ThirdBodyCalc m_falloff_concm;

    Rate1<Plog> m_plog_rates;
    Rate1<Chebyshev> m_cheb_rates;
    Rate1<BlowersMasel> m_blowersmasel_rates;

    //! @name Reaction rate data
    //!@{
    doublereal m_logp_ref;
    doublereal m_logc_ref;
    doublereal m_logStandConc;
    vector_fp m_rfn_low;
    vector_fp m_rfn_high;

    doublereal m_pres; //!< Last pressure at which rates were evaluated
    vector_fp falloff_work;
    vector_fp concm_3b_values;
    vector_fp concm_falloff_values;

    //!@}

    void processFalloffReactions();

    // functions marked as deprecated below are only used for XML import and
    // transitional reaction types are marked as '-legacy'

    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void addThreeBodyReaction(ThreeBodyReaction2& r);
    void addFalloffReaction(FalloffReaction& r);
    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void addPlogReaction(PlogReaction2& r);
    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void addChebyshevReaction(ChebyshevReaction2& r);
    void addBlowersMaselReaction(BlowersMaselReaction& r);

    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void modifyThreeBodyReaction(size_t i, ThreeBodyReaction2& r);
    void modifyFalloffReaction(size_t i, FalloffReaction& r);
    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void modifyPlogReaction(size_t i, PlogReaction2& r);
    //! @deprecated To be removed after Cantera 2.6 (replaced by MultiRate approach)
    void modifyChebyshevReaction(size_t i, ChebyshevReaction2& r);
    void modifyBlowersMaselReaction(size_t i, BlowersMaselReaction& r);

    //! Update the equilibrium constants in molar units.
    void updateKc();
};

}

#endif
