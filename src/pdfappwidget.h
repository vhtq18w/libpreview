#ifndef LIBPREVIEW_PDFAPPWIDGET_H
#define LIBPREVIEW_PDFAPPWIDGET_H

#include <QWidget>

struct PdfAppWidgetPrivate;

class PdfAppWidget : public QWidget {
  Q_OBJECT
signals:
  void pdfInitialize();

  void pdfLoaded();

private slots:
  void onLoadFinished(bool status);

  void renderPdfData();

  void renderPdfFile();

  void onRenderPdfFinished();
  
public:
  PdfAppWidget(QWidget *ptr_parent);

  ~PdfAppWidget();

  bool loadFile(const QString &path);

  void loadData(const QByteArray &data);

  void closePdf();

  QStringList getDestinations() const;

private:
  PdfAppWidgetPrivate *m_ptr_pdfAppWidgetPrivate;
};

#endif // LIBPREVIEW_PDFAPPWIDGET_H
