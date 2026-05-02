#include "ViewEditorsPage.h"
#include "EditorCardWidget.h"
#include "ConfigManager.h"
#include <QVBoxLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QCoreApplication>
#include <QPixmap>
#include <QIcon>
#include <algorithm>
#include <QSplitter>
#include "NodeManager.h"

ViewEditorsPage::ViewEditorsPage(QWidget* parent) : QWidget(parent) {
    QHBoxLayout* outer = new QHBoxLayout(this);
    outer->setContentsMargins(0,0,0,0);

    // Sidebar is now handled by MainWindow/mainPage. This view only shows grid of editors.

    QVBoxLayout* mainLayout = new QVBoxLayout();
    mainLayout->setContentsMargins(0, 0, 0, 0);

    // Back Button
    QPushButton* backButton = new QPushButton(this);
    // Try to load icon using robust data path helper
    QPixmap pixmap;
    bool iconLoaded = false;
    QString iconPath = NodeManager::getDataPath("assets/AR.png");
    if (pixmap.load(iconPath)) {
        backButton->setIcon(QIcon(pixmap));
        iconLoaded = true;
    }

    backButton->setFixedSize(44, 44);
    // Make icon / text clearly visible
    if (iconLoaded) {
        backButton->setIconSize(QSize(28, 28));
        backButton->setStyleSheet("border: none; background: transparent; color: #ffffff; padding-left: 6px;");
    } else {
        // Fallback to a visible arrow text if icon not found
        backButton->setText("\u2190");
        backButton->setStyleSheet("color: #ffffff; font-size: 22px; font-weight: bold; border: none; background: transparent; padding-left: 6px;");
    }
    connect(backButton, &QPushButton::clicked, this, &ViewEditorsPage::backRequested);

    QHBoxLayout* topLayout = new QHBoxLayout();
    topLayout->addWidget(backButton);
    topLayout->addStretch();
    topLayout->setContentsMargins(8, 8, 8, 8);
    mainLayout->addLayout(topLayout);

    // Scroll Area (main content)
    QScrollArea* scrollArea = new QScrollArea();
    scrollArea->setWidgetResizable(true);
    scrollArea->setFrameShape(QFrame::NoFrame);

    QWidget* scrollWidget = new QWidget();
    m_gridLayout = new QGridLayout(scrollWidget);
    m_gridLayout->setContentsMargins(16, 16, 16, 16);
    m_gridLayout->setSpacing(16);
    
    // Set 5 columns stretch
    for (int i = 0; i < 5; ++i) {
        m_gridLayout->setColumnStretch(i, 1);
    }

    scrollWidget->setLayout(m_gridLayout);
    scrollArea->setWidget(scrollWidget);

    // Keep main background black; scroll area will show cards on dark grey boxes
    mainLayout->addWidget(scrollArea);

    outer->addLayout(mainLayout);

    // Nodes directory relative to application dir / repo (kept for future use)
}

void ViewEditorsPage::clearLayout(QLayout* layout) {
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        if (item->widget()) {
            item->widget()->deleteLater();
        }
        delete item;
    }
}

void ViewEditorsPage::loadEditors() {
    clearLayout(m_gridLayout);

    QList<EditorEntry> entries = ConfigManager::loadEntries();
    
    // Entries are displayed in the order stored in the config file.
    // Favorites are moved to the front when they are toggled, so we
    // rely on the persisted ordering instead of re-sorting here.

    int row = 0;
    int col = 0;

    for (const auto& entry : entries) {
        // Ensure card widgets are parented to the scroll area's widget (the grid's parent)
        QWidget* gridParent = m_gridLayout->parentWidget();
        EditorCardWidget* card = new EditorCardWidget(entry, gridParent);
        connect(card, &EditorCardWidget::deletionRequested, this, &ViewEditorsPage::loadEditors);
        connect(card, &EditorCardWidget::favoriteChanged, this, &ViewEditorsPage::loadEditors);
        connect(card, &EditorCardWidget::nameChanged, this, &ViewEditorsPage::loadEditors);
        
        m_gridLayout->addWidget(card, row, col);
        col++;
        if (col >= 5) {
            col = 0;
            row++;
        }
    }
    
    // We add a stretchable spacer at the end to push cards to the top
    QSpacerItem* spacer = new QSpacerItem(0, 0, QSizePolicy::Minimum, QSizePolicy::Expanding);
    m_gridLayout->addItem(spacer, row + 1, 0, 1, 5);
}

// Simple fuzzy score wrapper (placeholder)
// legacy placeholder removed

// search functionality moved into NodeManager::filterNodes
