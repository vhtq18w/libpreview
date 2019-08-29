#ifndef LIBPREVIEW_PDFAPPBRIDGE_H
#define LIBPREVIEW_PDFAPPBRIDGE_H

#include <QPointer>
#include <QSemaphore>
#include <QWebEngineView>

namespace LibPreview {
class PdfBridgeObject;
class PdfAppBridge;
}

class PdfBridgeObject;

class PdfAppBridge : public QWebEngineView {
Q_OBJECT

  friend class PdfBridgeObject;

signals:
  void pdfInitialized();

  void pdfLoaded();

private slots:
  void onLoadFinished(bool status);

public:
  explicit PdfAppBridge(QWidget *parent = nullptr);

  ~PdfAppBridge();

  void invokeJavaScript(const QString &script);

  QVariant invokeJavaScriptAndWaitForResult(const QString &script);

  QStringList fetchPdfDocumentDestinations();

  void close();

private:
  static void jsInitialized();

  void jsReportDestinations(const QStringList &destinations);

  void jsLoaded();

  void jsClosed();

  void establishWebChannel();

private:
  QWebChannel *m_ptr_webChannel;

  QSemaphore m_pdfDestinationsSemaphore;

  QStringList m_pdfDocumentDestinations;

  QSemaphore m_pdfCloseSemaphore;

  PdfBridgeObject *m_ptr_pdfBridgeObject;
};

class PdfBridgeObject : public QObject {
Q_OBJECT
public:
  explicit PdfBridgeObject(PdfAppBridge *pAppBridge);

public slots:
  void jsInitialized();

  void jsReportDestinations(const QStringList &destinations);

  void jsLoaded();

  void jsClosed();

private:
  QPointer<PdfAppBridge> m_ptr_pdfAppBridge;
};

#endif // LIBPREVIEW_PDFAPPBRIDGE_H
