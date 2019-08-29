#include <QDebug>
#include <QFile>
#include <QFileInfo>
#include <QVBoxLayout>
#include "pdfappbridge.h"
#include "pdfappwidget.h"

const QString pdfPreviewAppUrl("qrc:/pdfapp/index.html");

////////////////////////////////////
// PdfAppWidgetPrivate Definition //
////////////////////////////////////

struct PdfAppWidgetPrivate {
  PdfAppBridge *ptr_pdfAppBridge;
  bool status;
  QByteArray pdfData;
  QString pdfFile;
};

PdfAppWidget::PdfAppWidget(QWidget *ptr_parent)
    : QWidget(ptr_parent) {
  m_ptr_pdfAppWidgetPrivate = new PdfAppWidgetPrivate;
  m_ptr_pdfAppWidgetPrivate->ptr_pdfAppBridge = new PdfAppBridge();
  connect(m_ptr_pdfAppWidgetPrivate->ptr_pdfAppBridge,
          &PdfAppBridge::pdfInitialized,
          this,
          &PdfAppWidget::pdfInitialize);
  connect(m_ptr_pdfAppWidgetPrivate->ptr_pdfAppBridge,
          &PdfAppBridge::loadFinished,
          this,
          &PdfAppWidget::onLoadFinished);
  connect(m_ptr_pdfAppWidgetPrivate->ptr_pdfAppBridge,
          &PdfAppBridge::pdfLoaded,
          this,
          &PdfAppWidget::onRenderPdfFinished);
  m_ptr_pdfAppWidgetPrivate->status = false;
  QUrl url = QUrl::fromUserInput(pdfPreviewAppUrl);
  auto ptr_layout = new QVBoxLayout();
  ptr_layout->setMargin(0);
  ptr_layout->addWidget(m_ptr_pdfAppWidgetPrivate->ptr_pdfAppBridge);
  setLayout(ptr_layout);
}

PdfAppWidget::~PdfAppWidget() {
  delete m_ptr_pdfAppWidgetPrivate;
}

bool
PdfAppWidget::loadFile(const QString &path) {
  QFileInfo fileInfo(path);
  try {
    if (fileInfo.exists()) {
      m_ptr_pdfAppWidgetPrivate->pdfFile = path;
      // TODO: choose transport a file path render pdf.
      QFile file(path);
      if (file.open(QIODevice::ReadOnly)) {
        QByteArray data = file.readAll();
        loadData(data);
      } else
        throw "Load file failed. Can't read pdf data.";
    } else
      throw "File not exist.";
  } catch (char *e) {
    qDebug() << e;
    return false;
  }
  return true;
}

void
PdfAppWidget::loadData(const QByteArray &data) {
  m_ptr_pdfAppWidgetPrivate->pdfData = data;
  renderPdfData();
}

void
PdfAppWidget::closePdf() {
  m_ptr_pdfAppWidgetPrivate->ptr_pdfAppBridge->close();
}

QStringList
PdfAppWidget::getDestinations() const {
  return m_ptr_pdfAppWidgetPrivate->ptr_pdfAppBridge->fetchPdfDocumentDestinations();
}

void
PdfAppWidget::onLoadFinished(bool status) {
  if (!status) {
    qCritical() << "Unable to initialize the web view";
    return;
  }
  m_ptr_pdfAppWidgetPrivate->status = true;
  if (!m_ptr_pdfAppWidgetPrivate->pdfData.isEmpty()) {
    renderPdfData();
  }
}

void
PdfAppWidget::renderPdfData() {
  if (m_ptr_pdfAppWidgetPrivate->status) {
    QString script = QString("previewPdfFromFileData('%1')")
      .arg(QString::fromUtf8(m_ptr_pdfAppWidgetPrivate->pdfData));
    m_ptr_pdfAppWidgetPrivate->ptr_pdfAppBridge->invokeJavaScript(script);
    m_ptr_pdfAppWidgetPrivate->pdfData.clear();
  }
}

// NOTE: This function need disable security check --disable-web-security
void
PdfAppWidget::renderPdfFile() {
  if (m_ptr_pdfAppWidgetPrivate->status) {
    QString script = QString("previewPdfFromFile('file://%1')")
      .arg(m_ptr_pdfAppWidgetPrivate->pdfFile);
    m_ptr_pdfAppWidgetPrivate->ptr_pdfAppBridge->invokeJavaScript(script);
    m_ptr_pdfAppWidgetPrivate->pdfData.clear();
  }
}

void
PdfAppWidget::onRenderPdfFinished() {
  emit pdfLoaded();
}
