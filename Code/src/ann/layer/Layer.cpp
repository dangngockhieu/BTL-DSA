#include "layer/ILayer.h"

ILayer::ILayer() {
    this->m_trainable = false;
}

ILayer::ILayer(const ILayer& orig) {
}

ILayer::~ILayer() {
}

unsigned long long ILayer::m_unLayer_idx =0;

