#include "NetworkRequestCreator.h"

NetworkRequestCreator::NetworkRequestCreator(std::unique_ptr<NetworkSourceContextPreparer> &&sourcePreparer)
    : m_sourcePreparer{sourcePreparer.release()}
{
    
}

bool NetworkRequestCreator::checkSourcePreparation(SourceBase *source)
{
    if (!source) return false;
    if (!m_sourcePreparer->prepareSourceContext(source->getContext().get())) return false;
    if (!m_sourcePreparer->prepareSource(source)) return false;
    
    return true;
}

template<>
bool NetworkRequestCreator::createRequestForSource<SourceStandardRSS>(SourceStandardRSS *source,
                                                                      QNetworkRequest &request)
{
    if (!source) return false;
    
    request = QNetworkRequest{source->getBaseUrl()};
    
    return true;
}

template<>
bool NetworkRequestCreator::createRequestForSource<SourceTelegram>(SourceTelegram *source,
                                                                   QNetworkRequest &request)
{
    checkSourcePreparation(source);
    
    auto context = source->getContext(); 
    auto telegramContext = dynamic_cast<SourceContextTelegram*>(context.get()); // FIXME: not clear
    
    QUrl requestUrl{source->getBaseUrl()};
    
    // getting channel messages...
    
    
    
    return true;
}

template<>
bool NetworkRequestCreator::createRequestForSource<SourceVK>(SourceVK *source,
                                                             QNetworkRequest &request)
{
    checkSourcePreparation(source);
    
    auto context = source->getContext(); 
    auto vkContext = dynamic_cast<SourceContextVK*>(context.get()); // FIXME: not clear
    
    QUrl requestUrl{source->getBaseUrl()};
    
    QUrlQuery getQuery{};
    
    getQuery.addQueryItem(SourceContextVK::C_TOKEN_PROPERTY,   vkContext->getToken());
    getQuery.addQueryItem(SourceContextVK::C_VERSION_PROPERTY, QString::number(SourceContextVK::C_VK_API_VERSION));
    
    requestUrl.setQuery(getQuery);
    request.setUrl(requestUrl);
    
    return true;
}

template<class SourceType>
bool NetworkRequestCreator::createRequestForSource(SourceType *source, 
                                                   QNetworkRequest &request)
{
    return false;
}