#include "uritests.h"

#include "guiutil.h"
#include "walletmodel.h"

#include <QUrl>

void URITests::uriTests()
{
    SendCoinsRecipient rv;
    QUrl uri;
    uri.setUrl(QString("auroracoin:AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v?req-dontexist="));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("auroracoin:AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v?dontexist="));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("auroracoin:AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v?label=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v"));
    QVERIFY(rv.label == QString("Wikipedia Example Address"));
    QVERIFY(rv.amount == 0);

    uri.setUrl(QString("auroracoin:AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v?amount=0.001"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100000);

    uri.setUrl(QString("auroracoin:AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v?amount=1.001"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v"));
    QVERIFY(rv.label == QString());
    QVERIFY(rv.amount == 100100000);

    uri.setUrl(QString("auroracoin:AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v?amount=100&label=Wikipedia Example"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v"));
    QVERIFY(rv.amount == 10000000000LL);
    QVERIFY(rv.label == QString("Wikipedia Example"));

    uri.setUrl(QString("auroracoin:AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v?message=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));
    QVERIFY(rv.address == QString("AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v"));
    QVERIFY(rv.label == QString());

    QVERIFY(GUIUtil::parseBitcoinURI("auroracoin://AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v?message=Wikipedia Example Address", &rv));
    QVERIFY(rv.address == QString("AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v"));
    QVERIFY(rv.label == QString());

    uri.setUrl(QString("auroracoin:AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v?req-message=Wikipedia Example Address"));
    QVERIFY(GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("auroracoin:AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v?amount=1,000&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));

    uri.setUrl(QString("auroracoin:AJgZfkqagmjajiB5pbAuXfh7yG9M5iMY9v?amount=1,000.0&label=Wikipedia Example"));
    QVERIFY(!GUIUtil::parseBitcoinURI(uri, &rv));
}
