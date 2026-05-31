#pragma once
#include <QDialog>
#include "NodeManager.h"

class NodeDetailDialog : public QDialog {
    Q_OBJECT
public:
    explicit NodeDetailDialog(const NodeEntry& entry, QWidget* parent = nullptr);
};
