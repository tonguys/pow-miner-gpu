/*
    This file is part of TON Blockchain Library.

    TON Blockchain Library is free software: you can redistribute it and/or modify
    it under the terms of the GNU Lesser General Public License as published by
    the Free Software Foundation, either version 2 of the License, or
    (at your option) any later version.

    TON Blockchain Library is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU Lesser General Public License for more details.

    You should have received a copy of the GNU Lesser General Public License
    along with TON Blockchain Library.  If not, see <http://www.gnu.org/licenses/>.

    Copyright 2017-2020 Telegram Systems LLP
*/
#pragma once
#include "td/db/KeyValue.h"
#include "vm/db/DynamicBagOfCellsDb.h"
#include "vm/cells.h"

#include "td/utils/Slice.h"
#include "td/utils/Status.h"

namespace vm {
using KeyValue = td::KeyValue;
using KeyValueReader = td::KeyValueReader;

class CellLoader {
 public:
  struct LoadResult {
   public:
    enum { Ok, NotFound } status{NotFound};

    Ref<DataCell> &cell() {
      DCHECK(status == Ok);
      return cell_;
    }

    td::int32 refcnt() const {
      return refcnt_;
    }

    Ref<DataCell> cell_;
    td::int32 refcnt_{0};
  };
  CellLoader(std::shared_ptr<KeyValueReader> reader);
  td::Result<LoadResult> load(td::Slice hash, bool need_data, ExtCellCreator &ext_cell_creator);

 private:
  std::shared_ptr<KeyValueReader> reader_;
};

class CellStorer {
 public:
  CellStorer(KeyValue &kv);
  td::Status erase(td::Slice hash);
  td::Status set(td::int32 refcnt, const DataCell &cell);

 private:
  KeyValue &kv_;
};
}  // namespace vm