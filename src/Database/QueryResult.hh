<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Database;

final class QueryResult
{

    private ImmVector<ImmMap<string, mixed>> $rows;

    public function __construct(
        Traversable<ImmMap<string, mixed>> $rows,
        private float $startTime,
        private float $endTime
    )
    {
        $this->rows = ImmVector::fromItems($rows);
    }

    public function rows(): ImmVector<ImmMap<string, mixed>>
    {
        return $this->rows;
    }

    public function isEmpty(): bool
    {
        return count($this->rows) <= 0;
    }

    public function startTime(): float
    {
        return $this->startTime;
    }

    public function endTime(): float
    {
        return $this->endTime;
    }

    public function pluck<Tu>(string $column, (function(mixed):Tu) $mapper): ImmSet<Tu>
    {
        $selector = (ImmMap<string, mixed> $row) ==> $mapper($row->at($column));
        return $this->rows->map($selector)->toImmSet();
    }

    public static function fromAsyncResult(\AsyncMysqlQueryResult $result) : this
    {
        $rows = $result->mapRowsTyped()
            ->map(($row) ==> $row->toImmMap())
            ->toImmVector();

        return new static($rows, $result->startTime(), $result->endTime());
    }

}
