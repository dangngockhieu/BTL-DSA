#include "loss/ILossLayer.h"

ILossLayer::ILossLayer(LossReduction reduction):
m_eReduction(reduction) {
}

ILossLayer::ILossLayer(const ILossLayer& orig):
m_eReduction(orig.m_eReduction){
}

ILossLayer::~ILossLayer() {
}

