#include "maditorviewlib.h"
#include "valuetypeedit.h"

#include <qtoolbutton>

VectorWidget::VectorWidget(int size) {
	QGridLayout *layout = new QGridLayout(this);
	layout->setSpacing(0);
	layout->setMargin(0);

	mBoxes.resize(size);

	for (int i = 0; i < size; ++i) {
		QDoubleSpinBox *box = new QDoubleSpinBox(this);
		layout->addWidget(box, 0, i);
		mBoxes[i] = box;
	}
}

void VectorWidget::setValue(const Engine::Vector3 & v)
{
	assert(mBoxes.size() == 3);
	for (int i = 0; i < 3; ++i) {
		mBoxes[i]->setValue(v[i]);
	}
}

void VectorWidget::setReadOnly(bool b)
{
	for (QDoubleSpinBox *box : mBoxes)
		box->setReadOnly(b);
}

const Engine::ValueType & ValueTypeEdit::getValue()
{
	return mValue;
}

ValueTypeEdit::ValueTypeEdit(QWidget *parent) :
    QWidget(parent),
	mInputWidget(nullptr),
	mReadOnly(false)
{

	mLayout = new QGridLayout(this);
	mLayout->setSpacing(0);
	mLayout->setMargin(0);

	mOuterMenu = new QToolButton;
	mOuterMenu->setPopupMode(QToolButton::InstantPopup);
	mOuterMenu->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

	mTypeMenu = new QMenu(this);

	

	mOuterMenu->setMenu(mTypeMenu);

	mLayout->addWidget(mOuterMenu, 0, 1, Qt::AlignRight);

	for (Engine::ValueType::Type type : {
			Engine::ValueType::Type::NullValue,
			Engine::ValueType::Type::IntValue,
			Engine::ValueType::Type::Vector3Value
	}) {
		mTypeMenu->addAction(QString::fromStdString(Engine::ValueType::getTypeString(type)), this, [=]() { setType(type); });
	}

	onTypeChanged();

}

ValueTypeEdit::~ValueTypeEdit()
{

}

void ValueTypeEdit::setType(Engine::ValueType::Type type) {
	if (mValue.type() != type) {
		switch (type) {
		case Engine::ValueType::Type::NullValue:
			mValue.clear();
			break;
		case Engine::ValueType::Type::IntValue:
			mValue = static_cast<int>(0);
			break;
		case Engine::ValueType::Type::Vector3Value:
			mValue = Engine::Vector3{};
			break;
		case Engine::ValueType::Type::UIntValue:
			mValue = static_cast<size_t>(0);
			break;
		case Engine::ValueType::Type::BoolValue:
			mValue = false;
			break;
		default:
			throw 0;
		}
		onTypeChanged();
	}
}

void ValueTypeEdit::setValue(const Engine::ValueType &value) {
	bool typeChanged = value.type() != mValue.type();
	mValue = value;
	if (typeChanged) onTypeChanged();
	else onValueChanged();
}

void ValueTypeEdit::setReadOnly(bool b)
{
	mOuterMenu->setVisible(!b);
	mReadOnly = b;

	switch (mValue.type()) {
	case Engine::ValueType::Type::NullValue:
		break;
	case Engine::ValueType::Type::IntValue:
		mIntBox->setReadOnly(b);		
		break;
	case Engine::ValueType::Type::Vector3Value:
		mVector3Box->setReadOnly(b);
		break;
	case Engine::ValueType::Type::InvScopePtrValue:
		break;
	case Engine::ValueType::Type::UIntValue:
		mUIntBox->setReadOnly(b);
		break;
	case Engine::ValueType::Type::BoolValue:
		mBoolBox->setEnabled(!b);
		break;
	default:
		throw 0;
	}
}

void ValueTypeEdit::onTypeChanged() {
	if (mInputWidget)
		mInputWidget->deleteLater();

	switch (mValue.type()) {
	case Engine::ValueType::Type::NullValue:
		mNullLabel = new QLabel("NULL", this);
		mInputWidget = mNullLabel;
		break;
	case Engine::ValueType::Type::IntValue:
		mIntBox = new QSpinBox(this);
		mIntBox->setMinimum(std::numeric_limits<int>::min());
		mIntBox->setMaximum(std::numeric_limits<int>::max());
		mInputWidget = mIntBox;
		break;
	case Engine::ValueType::Type::Vector3Value:
		mVector3Box = new VectorWidget(3);
		mInputWidget = mVector3Box;
		break;
	case Engine::ValueType::Type::InvScopePtrValue:
		mScopeField = new QPushButton("->", this);
		connect(mScopeField, &QPushButton::clicked, this, &ValueTypeEdit::followScopeLink);
		mInputWidget = mScopeField;
		break;
	case Engine::ValueType::Type::UIntValue:
		mUIntBox = new QSpinBox(this);
		mUIntBox->setMinimum(0);
		mUIntBox->setMaximum(std::numeric_limits<int>::max());
		mInputWidget = mUIntBox;
		break;
	case Engine::ValueType::Type::BoolValue:
		mBoolBox = new QCheckBox(this);
		mInputWidget = mBoolBox;
		break;
	default:
		throw 0;
	}

	mInputWidget->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Preferred);
	setReadOnly(mReadOnly);
	mLayout->addWidget(mInputWidget, 0, 0);

	onValueChanged();
}

void ValueTypeEdit::onValueChanged() {
	switch (mValue.type()) {
	case Engine::ValueType::Type::NullValue:
		break;
	case Engine::ValueType::Type::IntValue:
		mIntBox->setValue(mValue.as<int>());
		break;
	case Engine::ValueType::Type::Vector3Value:
		mVector3Box->setValue(mValue.as<Engine::Vector3>());
		break;
	case Engine::ValueType::Type::InvScopePtrValue:
		break;
	case Engine::ValueType::Type::UIntValue:
		mUIntBox->setValue(mValue.as<size_t>());
		break;
	case Engine::ValueType::Type::BoolValue:
		mBoolBox->setChecked(mValue.as<bool>());
		break;
	default:
		throw 0;
	}
}

void ValueTypeEdit::followScopeLink()
{
	emit scopeLinkClicked(mValue.as<Engine::InvScopePtr>());
}