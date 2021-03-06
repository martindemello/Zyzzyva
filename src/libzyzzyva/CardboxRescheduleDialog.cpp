//---------------------------------------------------------------------------
// CardboxRescheduleDialog.cpp
//
// A dialog for rescheduling words within the cardbox system.
//
// Copyright 2006-2012 Boshvark Software, LLC.
//
// This file is part of Zyzzyva.
//
// Zyzzyva is free software; you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation; either version 2 of the License, or
// (at your option) any later version.
//
// Zyzzyva is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program; if not, write to the Free Software
// Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
//---------------------------------------------------------------------------

#include "CardboxRescheduleDialog.h"
#include "LexiconSelectWidget.h"
#include "SearchSpecForm.h"
#include "ZPushButton.h"
#include "Auxil.h"
#include "Defs.h"
#include <QButtonGroup>
#include <QFrame>
#include <QLabel>
#include <QVBoxLayout>

const QString DIALOG_CAPTION = "Reschedule Words in Cardbox";

using namespace Defs;

//---------------------------------------------------------------------------
//  CardboxRescheduleDialog
//
//! Constructor.
//
//! @param parent the parent widget
//! @param f widget flags
//---------------------------------------------------------------------------
CardboxRescheduleDialog::CardboxRescheduleDialog(QWidget* parent, Qt::WFlags f)
    : QDialog(parent, f)
{
    QVBoxLayout* mainVlay = new QVBoxLayout(this);
    mainVlay->setMargin(MARGIN);
    mainVlay->setSpacing(SPACING);

    lexiconWidget = new LexiconSelectWidget;
    mainVlay->addWidget(lexiconWidget);

    QHBoxLayout* quizTypeHlay = new QHBoxLayout;
    mainVlay->addLayout(quizTypeHlay);

    QLabel* quizTypeLabel = new QLabel;
    quizTypeLabel->setText("Reschedule words for quiz type:");
    quizTypeHlay->addWidget(quizTypeLabel);

    quizTypeCombo = new QComboBox;
    quizTypeCombo->addItem(Auxil::quizTypeToString(QuizSpec::QuizAnagrams));
    quizTypeCombo->addItem(
        Auxil::quizTypeToString(QuizSpec::QuizAnagramsWithHooks));
    quizTypeCombo->addItem(Auxil::quizTypeToString(QuizSpec::QuizHooks));
    quizTypeHlay->addWidget(quizTypeCombo);

    QButtonGroup* methodGroup = new QButtonGroup(this);

    QHBoxLayout* backlogHlay = new QHBoxLayout;
    mainVlay->addLayout(backlogHlay);

    shiftQuestionsButton = new QRadioButton;
    shiftQuestionsButton->setText("Shift words so this many are ready now:");
    connect(shiftQuestionsButton, SIGNAL(toggled(bool)),
            SLOT(shiftQuestionsButtonToggled(bool)));
    methodGroup->addButton(shiftQuestionsButton);
    backlogHlay->addWidget(shiftQuestionsButton);

    backlogSbox = new QSpinBox;
    backlogSbox->setMinimum(1);
    backlogSbox->setMaximum(999999);
    backlogHlay->addWidget(backlogSbox);

    rescheduleQuestionsButton = new QRadioButton;
    rescheduleQuestionsButton->setText("Reschedule words according to "
                                       "their cardbox");
    methodGroup->addButton(rescheduleQuestionsButton);
    mainVlay->addWidget(rescheduleQuestionsButton);

    QFrame* hline = new QFrame;
    hline->setFrameShape(QFrame::HLine);
    mainVlay->addWidget(hline);

    QButtonGroup* selectGroup = new QButtonGroup(this);

    selectAllButton = new QRadioButton;
    selectAllButton->setText("Reschedule all words");
    selectGroup->addButton(selectAllButton);
    mainVlay->addWidget(selectAllButton);

    selectSearchButton = new QRadioButton;
    connect(selectSearchButton, SIGNAL(toggled(bool)),
            SLOT(useSearchButtonToggled(bool)));
    selectSearchButton->setText("Reschedule only words matching search "
                                "specification");
    selectGroup->addButton(selectSearchButton);
    mainVlay->addWidget(selectSearchButton);

    searchSpecGbox = new QGroupBox("Search Specification");
    mainVlay->addWidget(searchSpecGbox);

    QHBoxLayout* specHlay = new QHBoxLayout(searchSpecGbox);

    searchSpecForm = new SearchSpecForm;
    specHlay->addWidget(searchSpecForm);

    QHBoxLayout* buttonHlay = new QHBoxLayout;
    buttonHlay->setSpacing(SPACING);
    mainVlay->addLayout(buttonHlay);

    ZPushButton* okButton = new ZPushButton("&OK");
    okButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    okButton->setDefault(true);
    connect(okButton, SIGNAL(clicked()), SLOT(accept()));
    buttonHlay->addWidget(okButton);

    ZPushButton* cancelButton = new ZPushButton("Cancel");
    cancelButton->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    connect(cancelButton, SIGNAL(clicked()), SLOT(reject()));
    buttonHlay->addWidget(cancelButton);

    setWindowTitle(DIALOG_CAPTION);
    shiftQuestionsButton->setChecked(true);
    selectAllButton->setChecked(true);
    searchSpecGbox->setEnabled(false);
}

//---------------------------------------------------------------------------
//  getLexicon
//
//! Return the lexicon chosen by the user.
//
//! @return the lexicon
//---------------------------------------------------------------------------
QString
CardboxRescheduleDialog::getLexicon() const
{
    return lexiconWidget->getCurrentLexicon();
}

//---------------------------------------------------------------------------
//  getQuizType
//
//! Return the quiz type chosen by the user.
//
//! @return the quiz type
//---------------------------------------------------------------------------
QString
CardboxRescheduleDialog::getQuizType() const
{
    return quizTypeCombo->currentText();
}

//---------------------------------------------------------------------------
//  getShiftQuestions
//
//! Determine whether questions are to be shifted, as opposed to completely
//! rescheduled.
//
//! @return true if questions should be shifted, false otherwise
//---------------------------------------------------------------------------
bool
CardboxRescheduleDialog::getShiftQuestions() const
{
    return shiftQuestionsButton->isChecked();
}

//---------------------------------------------------------------------------
//  getBacklogSize
//
//! Determine the desired backlog size.
//
//! @return the backlog size
//---------------------------------------------------------------------------
int
CardboxRescheduleDialog::getBacklogSize() const
{
    return backlogSbox->value();
}

//---------------------------------------------------------------------------
//  getRescheduleAll
//
//! Determine whether all questions are to be rescheduled.
//
//! @return true if all questions should be rescheduled, false otherwise
//---------------------------------------------------------------------------
bool
CardboxRescheduleDialog::getRescheduleAll() const
{
    return selectAllButton->isChecked();
}

//---------------------------------------------------------------------------
//  getSearchSpec
//
//! Determine whether all questions are to be rescheduled
//
//! @return true if all questions should be rescheduled
//---------------------------------------------------------------------------
SearchSpec
CardboxRescheduleDialog::getSearchSpec() const
{
    return searchSpecForm->getSearchSpec();
}

//---------------------------------------------------------------------------
//  shiftQuestionsButtonToggled
//
//! Called when the state of the Shift Backlog button is toggled.  Enable or
//! disable the backlog size spin box.
//
//! @param checked true if the button is checked
//---------------------------------------------------------------------------
void
CardboxRescheduleDialog::shiftQuestionsButtonToggled(bool checked)
{
    backlogSbox->setEnabled(checked);
}

//---------------------------------------------------------------------------
//  useSearchButtonToggled
//
//! Called when the state of the Use Search button is toggled.  Enable or
//! disable the search specification form.
//
//! @param checked true if the button is checked
//---------------------------------------------------------------------------
void
CardboxRescheduleDialog::useSearchButtonToggled(bool checked)
{
    searchSpecGbox->setEnabled(checked);
}
