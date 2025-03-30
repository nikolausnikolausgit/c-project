#include <QApplication>
#include <QWidget>
#include <QVBoxLayout>
#include <QFormLayout>
#include <QLineEdit>
#include <QLabel>
#include <QPushButton>
#include <QMessageBox>
#include <QDoubleValidator>
#include <cmath>

class WedgeProblem : public QWidget {
    Q_OBJECT
public:
    WedgeProblem(QWidget *parent = nullptr) : QWidget(parent) {
        QLabel *problemLabel = new QLabel(
            "<h2 style='color: #2c3e50; margin-bottom: 10px;'>Закон сохранения импульса</h2>"
            "<div style='background-color: #f5f5f5; padding: 15px; border-radius: 5px; border: 1px solid #ddd; margin-bottom: 15px;'>"
            "<p style='margin-top: 0;'><b>Условие:</b> Клин массой <b>M</b> с углом наклона <b>α</b> находится на гладкой поверхности. "
            "Брусок массой <b>m</b> соскальзывает с вершины высотой <b>H</b>, упруго ударяется о стенку и возвращается обратно.</p>"
            "<p><b>Требуется найти:</b></p>"
            "<ol>"
            "<li>Скорость клина <b>V</b> при первом достижении бруском основания</li>"
            "<li>Максимальную высоту <b>h</b> после столкновения</li>"
            "<li>Полное время движения <b>t</b> до возврата на вершину</li>"
            "</ol>"
            "</div>"
            );
        problemLabel->setWordWrap(true);


        MInput = createInputField("Масса клина M (кг):");
        mInput = createInputField("Масса бруска m (кг):");
        alphaInput = createInputField("Угол наклона α (град):");
        HInput = createInputField("Высота клина H (м):");

        QPushButton *calculateButton = new QPushButton("Рассчитать");
        calculateButton->setStyleSheet(
            "QPushButton {"
            "  background-color: #3498db;"
            "  color: white;"
            "  border: none;"
            "  padding: 10px 20px;"
            "  border-radius: 5px;"
            "  font-size: 14px;"
            "}"
            "QPushButton:hover {"
            "  background-color: #2980b9;"
            "}"
            );
        connect(calculateButton, &QPushButton::clicked, this, &WedgeProblem::calculate);


        QLabel *resultsHeader = new QLabel("<h3 style='color: #2c3e50; margin-top: 15px;'>Результаты:</h3>");
        velocityLabel = createResultLabel("Скорость клина V:");
        heightLabel = createResultLabel("Высота подъема h:");
        timeLabel = createResultLabel("Полное время t:");

        QVBoxLayout *mainLayout = new QVBoxLayout;
        mainLayout->setContentsMargins(20, 20, 20, 20);
        mainLayout->setSpacing(15);
        mainLayout->addWidget(problemLabel);

        QFormLayout *inputLayout = new QFormLayout;
        inputLayout->setSpacing(10);
        inputLayout->addRow(MInput->placeholderText(), MInput);
        inputLayout->addRow(mInput->placeholderText(), mInput);
        inputLayout->addRow(alphaInput->placeholderText(), alphaInput);
        inputLayout->addRow(HInput->placeholderText(), HInput);

        mainLayout->addLayout(inputLayout);
        mainLayout->addWidget(calculateButton, 0, Qt::AlignCenter);
        mainLayout->addWidget(resultsHeader);
        mainLayout->addWidget(velocityLabel);
        mainLayout->addWidget(heightLabel);
        mainLayout->addWidget(timeLabel);
        mainLayout->addStretch();

        setLayout(mainLayout);
        setWindowTitle("Задача №4, МЭ ВСОШ");
        setMinimumSize(550, 600);
    }

private slots:
    void calculate() {
        try {
            double M = getPositiveValue(MInput, "масса клина M");
            double m = getPositiveValue(mInput, "масса бруска m");
            double alpha_deg = getAngleValue(alphaInput);
            double H = getPositiveValue(HInput, "высота клина H");

            double alpha = alpha_deg * M_PI / 180.0;
            double sin_a = sin(alpha);
            double cos_a = cos(alpha);

            double V = sqrt(2*m*m*9.81*H*cos_a*cos_a/((M + m)*(M + m*sin_a*sin_a)));

            double h = H * pow((M - m*sin_a*sin_a)/(M + m*sin_a*sin_a), 2);

            double t = 2*sqrt(2*H/(9.81*sin_a))*(1 + (M + m*sin_a*sin_a)/M);

            velocityLabel->setText(QString("Скорость клина V: <b>%1 м/с</b>").arg(V, 0, 'f', 3));
            heightLabel->setText(QString("Высота подъема h: <b>%1 м</b>").arg(h, 0, 'f', 3));
            timeLabel->setText(QString("Полное время t: <b>%1 с</b>").arg(t, 0, 'f', 2));

        } catch (const std::exception &e) {
            QMessageBox::warning(this, "Ошибка", e.what());
        }
    }

private:
    QLineEdit *MInput, *mInput, *alphaInput, *HInput;
    QLabel *velocityLabel, *heightLabel, *timeLabel;

    QLineEdit* createInputField(const QString& placeholder) {
        QLineEdit* input = new QLineEdit;
        input->setPlaceholderText(placeholder);
        input->setValidator(new QDoubleValidator(0.0, 1000.0, 5, this));
        input->setStyleSheet(
            "QLineEdit {"
            "  padding: 8px;"
            "  border: 1px solid #ccc;"
            "  border-radius: 4px;"
            "  min-width: 200px;"
            "}"
            );
        return input;
    }

    QLabel* createResultLabel(const QString& text) {
        QLabel* label = new QLabel(text);
        label->setStyleSheet(
            "QLabel {"
            "  margin: 8px 0;"
            "  padding: 5px;"
            "  font-size: 14px;"
            "}"
            );
        return label;
    }

    double getPositiveValue(QLineEdit* input, const QString& name) {
        bool ok;
        double value = input->text().toDouble(&ok);
        if (!ok || value <= 0) {
            throw std::invalid_argument(QString("Некорректное значение для %1").arg(name).toStdString());
        }
        return value;
    }

    double getAngleValue(QLineEdit* input) {
        bool ok;
        double angle = input->text().toDouble(&ok);
        if (!ok || angle <= 0 || angle >= 90) {
            throw std::invalid_argument("Угол α должен быть: 0° < α < 90°");
        }
        return angle;
    }
};

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    WedgeProblem solver;
    solver.show();
    return app.exec();
}


#include "main.moc"
