/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Migration;

use HHPack\Migrate\{MigrationName};
use HHPack\Migrate\Database\{QueryResult};

final class MigrationResult {

  public function __construct(
    private ImmMap<MigrationName, ImmVector<QueryResult>> $results,
  ) {}

  public function results(
  ): ImmVector<Pair<MigrationName, ImmVector<QueryResult>>> {
    return ImmVector::fromItems($this->results->items());
  }

  public function at(MigrationName $key): ImmVector<QueryResult> {
    return $this->results->at($key);
  }

  public function containsKey(MigrationName $key): bool {
    return $this->results->containsKey($key);
  }

  public function resultCount(): int {
    return $this->results->count();
  }

}
