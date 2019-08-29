#include <QApplication>
#include <QWebChannel>

#include "pdfappbridge.h"

/////////////////////////////////
// PdfAppBridge implementation //
/////////////////////////////////

void
PdfAppBridge::onLoadFinished(bool status) {
  if (status) {
    establishWebChannel();
    emit pdfInitialized();
  }
}

PdfAppBridge::PdfAppBridge(QWidget *parent)
    : QWebEngineView(parent) {
  m_ptr_webChannel = new QWebChannel(this);
  m_ptr_pdfBridgeObject = new PdfBridgeObject(this);
  connect(this, &QWebEngineView::loadFinished, this, &PdfAppBridge::onLoadFinished);
}

PdfAppBridge::~PdfAppBridge() {
  delete m_ptr_webChannel;
  // TODO: delete other pointer?
}

void
PdfAppBridge::invokeJavaScript(const QString &script) {
  QWebEnginePage *ptr_page = page();
  if (ptr_page != nullptr)
    ptr_page->runJavaScript(script);
}

QVariant
PdfAppBridge::invokeJavaScriptAndWaitForResult(const QString &script) {
  QVariant result;
  QWebEnginePage *ptr_page = page();
  if (ptr_page != nullptr) {
    QSemaphore waitSemaphore;
    ptr_page->runJavaScript(script, [&result, &waitSemaphore](const QVariant &res) {
      result = res;
      waitSemaphore.release();
    });
    while (!waitSemaphore.tryAcquire()) {
      qApp->processEvents();
    }
  }
  return result;
}

QStringList
PdfAppBridge::fetchPdfDocumentDestinations() {
  m_pdfDocumentDestinations.clear();
  invokeJavaScript("pdfAppFetchDestinations();");
  while (!m_pdfDestinationsSemaphore.tryAcquire()) {
    qApp->processEvents();
  }
  return m_pdfDocumentDestinations;
}

void
PdfAppBridge::close() {
  invokeJavaScript("pdfAppClose();");
  while (!m_pdfCloseSemaphore.tryAcquire()) {
    qApp->processEvents();
  }
}

void
PdfAppBridge::jsInitialized() {
  // App web view have initialized.
}

void
PdfAppBridge::jsReportDestinations(const QStringList &destinations) {
  m_pdfDocumentDestinations = destinations;
  m_pdfDestinationsSemaphore.release();
}

void
PdfAppBridge::jsLoaded() {
  emit pdfLoaded();
}

void
PdfAppBridge::jsClosed() {
  m_pdfCloseSemaphore.release();
}

void
PdfAppBridge::establishWebChannel() {
  QWebEnginePage *ptr_page = page();
  if (ptr_page != nullptr) {
    ptr_page->setWebChannel(m_ptr_webChannel);
    m_ptr_webChannel->registerObject(QStringLiteral("pdfAppBridge"), m_ptr_pdfBridgeObject);
    ptr_page->runJavaScript("pdfInitialize();");
  }
}

////////////////////////////////////
// PdfBridgeObject implementation //
////////////////////////////////////
PdfBridgeObject::PdfBridgeObject(PdfAppBridge *pAppBridge)
    : QObject(pAppBridge), m_ptr_pdfAppBridge(pAppBridge) {
  Q_ASSERT(pAppBridge != nullptr);
}

void
PdfBridgeObject::jsInitialized() {
  m_ptr_pdfAppBridge->jsInitialized();
}

void
PdfBridgeObject::jsReportDestinations(const QStringList &destinations) {
  m_ptr_pdfAppBridge->jsReportDestinations(destinations);
}

void
PdfBridgeObject::jsLoaded() {
  m_ptr_pdfAppBridge->jsLoaded();
}

void
PdfBridgeObject::jsClosed() {
  m_ptr_pdfAppBridge->jsClosed();
}
