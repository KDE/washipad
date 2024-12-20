# SPDX-FileCopyrightText: 2024 Carl Schwan <carlschwan@kde.org>
# SPDX-License-Identifier: BSD-2-Clause

ecm_add_test(sketchmodeltest.cpp LINK_LIBRARIES Qt::Test washipadlib)
ecm_add_test(stroketest.cpp LINK_LIBRARIES Qt::Test washipadlib)
