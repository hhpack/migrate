<?hh //strict

/**
 * This file is part of hhpack\migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace hhpack\migrate;

final class MigrationResult
{

    public function __construct(
        private ImmVector<QueryResult> $results
    )
    {
    }

    public function results(): ImmVector<QueryResult>
    {
        return $this->results;
    }

    public function at(int $index): QueryResult
    {
        return $this->results->at($index);
    }

    public function resultCount(): int
    {
        return $this->results->count();
    }

}
