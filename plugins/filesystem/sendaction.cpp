/*
 * The MIT License (MIT)
 *
 * Copyright (c) 2017 Nathan Osman
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to
 * deal in the Software without restriction, including without limitation the
 * rights to use, copy, modify, merge, publish, distribute, sublicense, and/or
 * sell copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
 * FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS
 * IN THE SOFTWARE.
 */

#include <QDir>
#include <QFileInfo>

#include <nitroshare/application.h>
#include <nitroshare/bundle.h>
#include <nitroshare/devicemodel.h>
#include <nitroshare/handler.h>
#include <nitroshare/handlerregistry.h>

#include "file.h"
#include "sendaction.h"

SendAction::SendAction(Application *application)
    : mApplication(application)
{
}

QString SendAction::name() const
{
    return "send";
}

QVariant SendAction::invoke(const QVariantMap &params)
{
    // Attempt to retrieve the device identified by the given name
    QString deviceName = params.value("device").toString();

    // TODO

    // Retrieve the root and list of items to add
    QDir root(params.value("root").toString());
    QStringList items = params.value("items").toStringList();

    // TODO: symlinks not handled correctly below

    // Create a bundle with the items added
    Bundle *bundle = new Bundle();
    foreach (QString item, items) {
        QFileInfo info(root.absoluteFilePath(item));
        bundle->addItem(new File(root, info, 0));
    }

    return true;
}
