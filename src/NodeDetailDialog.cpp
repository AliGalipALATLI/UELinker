#include "NodeDetailDialog.h"
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>
#include <QToolButton>
#include <QIcon>
#include <QCoreApplication>

NodeDetailDialog::NodeDetailDialog(const NodeEntry& entry, QWidget* parent)
    : QDialog(parent) {
    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    setModal(true);
    setMinimumSize(480, 320);

    QVBoxLayout* main = new QVBoxLayout(this);
    main->setContentsMargins(8, 8, 8, 8);

    // Top toolbar with back arrow (close)
    QHBoxLayout* top = new QHBoxLayout();
    QToolButton* back = new QToolButton(this);
    back->setText("\u2190");
    back->setAutoRaise(true);
    back->setToolButtonStyle(Qt::ToolButtonTextOnly);
    connect(back, &QToolButton::clicked, this, &NodeDetailDialog::accept);
    top->addWidget(back);
    top->addStretch();
    main->addLayout(top);

    QLabel* title = new QLabel(entry.name, this);
    QFont f = title->font();
    f.setPointSize(14);
    f.setBold(true);
    title->setFont(f);
    main->addWidget(title);

    QLabel* desc = new QLabel(entry.description, this);
    desc->setWordWrap(true);
    main->addWidget(desc);

    main->addStretch();
}
