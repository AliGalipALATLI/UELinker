#include "EditorCardWidget.h"
#include "ConfigManager.h"
#include "DesktopEntryWriter.h"
#include <QHBoxLayout>
#include <QMenu>
#include <QMessageBox>
#include <QDir>
#include <QPixmap>
#include <QCoreApplication>
#include <QIcon>

EditorCardWidget::EditorCardWidget(const EditorEntry& entry, QWidget* parent)
    : QWidget(parent), m_entry(entry) {
    
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);

    QHBoxLayout* layout = new QHBoxLayout(this);
    layout->setContentsMargins(8, 8, 8, 8);

    QString appDir = QCoreApplication::applicationDirPath();

    m_nameLabel = new QLabel(entry.name, this);
    m_nameLabel->setStyleSheet("color: #ffffff; padding-left: 8px;");

    // Delete button inside the card (on the right)
    m_deleteButton = new QPushButton(this);
    QPixmap delPix;
    if (!delPix.load(appDir + "/assets/TRA.png")) {
        delPix.load("assets/TRA.png");
    }
    if (!delPix.isNull()) {
        m_deleteButton->setIcon(QIcon(delPix));
        m_deleteButton->setIconSize(QSize(20, 20));
    } else {
        m_deleteButton->setText("Del"); // fallback text if icon not available
    }
    m_deleteButton->setFixedSize(36, 36);
    m_deleteButton->setCursor(Qt::PointingHandCursor);
    m_deleteButton->setStyleSheet("border: none; background: transparent; color: #ffffff;");

    // Layout: name label expands, delete button at right inside the same card
    layout->addWidget(m_nameLabel, 1);
    layout->addSpacing(8);
    layout->addWidget(m_deleteButton, 0, Qt::AlignRight | Qt::AlignVCenter);

    // Make the card a bit wider so delete button is clearly inside the card
    setMinimumWidth(220);

    // Card visual style: dark grey background with rounded corners
    this->setStyleSheet("background: #2b2b2b; border: 1px solid #3a3a3a; border-radius: 8px;");
    this->setContentsMargins(0,0,0,0);

    connect(m_deleteButton, &QPushButton::clicked, this, &EditorCardWidget::showDeleteMenu);
}

void EditorCardWidget::showDeleteMenu() {
    QMenu menu(this);
    
    QAction* deleteAppAction = menu.addAction("Delete App");
    QAction* deleteFilesAction = menu.addAction("Delete Files");

    // Use cursor position for menu placement to ensure it appears where the user clicked
    QAction* selectedAction = menu.exec(QCursor::pos());

    if (selectedAction == deleteAppAction) {
        DesktopEntryWriter::remove(m_entry.name);
        ConfigManager::removeEntry(m_entry.name);
        emit deletionRequested();
    } else if (selectedAction == deleteFilesAction) {
        QMessageBox::StandardButton reply;
        reply = QMessageBox::warning(this, "Warning",
            QString("This will permanently delete all engine source files at:\n%1\nThis action cannot be undone.").arg(m_entry.path),
            QMessageBox::Ok | QMessageBox::Cancel);
        
        if (reply == QMessageBox::Ok) {
            DesktopEntryWriter::remove(m_entry.name);
            QDir dir(m_entry.path);
            if (dir.exists()) {
                dir.removeRecursively();
            }
            ConfigManager::removeEntry(m_entry.name);
            emit deletionRequested();
        }
    }
}
