<?hh //strict

namespace hhpack\migrate;

interface Connection<T>
{
    public function query(string $query): Awaitable<T>;
    public function close(): void;
}
