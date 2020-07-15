// -*- C++ -*-
//
// Package:     Core
// Class  :     unittest_changemanager
//
// Implementation:
//     <Notes on implementation>
//
// Original Author:  Chris Jones
//         Created:  Fri Jan 18 10:19:07 EST 2008
//

// system include files
#include <boost/test/auto_unit_test.hpp>
#include <boost/test/test_tools.hpp>
#include "TClass.h"

// user include files
#include "DataFormats/TrackReco/interface/TrackFwd.h"
#include "DataFormats/TrackReco/interface/Track.h"
#include "FWCore/Reflection/interface/ObjectWithDict.h"
#include "Fireworks/Core/interface/FWEventItem.h"

#include "Fireworks/Core/interface/FWModelChangeManager.h"

#include "Fireworks/Core/interface/FWSelectionManager.h"
#include "Fireworks/Core/interface/FWModelExpressionSelector.h"

#include "Fireworks/Core/interface/FWItemAccessorBase.h"

//
// constants, enums and typedefs
//

namespace {
  class TestAccessor : public FWItemAccessorBase {
  public:
    TestAccessor(const reco::TrackCollection* iCollection) : m_collection(iCollection) {}
    const void* modelData(int iIndex) const override { return &((*m_collection)[iIndex]); }
    const void* data() const override { return m_collection; }
    unsigned int size() const override { return m_collection->size(); }
    const TClass* modelType() const override { return TClass::GetClass("reco::Track"); }
    const TClass* type() const override { return TClass::GetClass("std::vector<reco::Track>"); }

    bool isCollection() const override { return true; }

    ///override if id of an object should be different than the index
    //virtual std::string idForIndex(int iIndex) const;
    // ---------- member functions ---------------------------
    void setData(const edm::ObjectWithDict&) override {}
    void reset() override {}

  private:
    const reco::TrackCollection* m_collection;
  };
}  // namespace

BOOST_AUTO_TEST_CASE(modelexpressionselector) {
  FWModelChangeManager cm;

  FWSelectionManager sm(&cm);

  reco::TrackCollection fVector;
  fVector.push_back(reco::Track());
  fVector.push_back(
      reco::Track(20, 20, reco::Track::Point(), reco::Track::Vector(20, 0, 0), -1, reco::Track::CovarianceMatrix()));
  fVector.push_back(reco::Track());

  TClass* cls = TClass::GetClass("std::vector<reco::Track>");
  assert(nullptr != cls);

  fireworks::Context context(&cm, &sm, nullptr, nullptr, nullptr);

  auto accessor = std::make_shared<TestAccessor>(&fVector);
  FWPhysicsObjectDesc pObj("Tracks", cls, "Tracks");

  FWEventItem item(&context, 0, accessor, pObj);
  //hack to force update of data
  edm::ObjectWithDict dummy;
  item.setData(dummy);

  cm.newItemSlot(&item);

  FWModelExpressionSelector selector;
  {
    FWChangeSentry sentry(cm);
    selector.select(&item, "$.pt() > 10");
  }
  BOOST_CHECK(1 == sm.selected().size());
  BOOST_CHECK(not item.modelInfo(0).m_isSelected);
  BOOST_CHECK(item.modelInfo(1).m_isSelected);
  BOOST_CHECK(not item.modelInfo(2).m_isSelected);
}
