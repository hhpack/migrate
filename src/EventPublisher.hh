<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate;

use HHPack\Publisher\Message;
use HHPack\Publisher\Subscribable;
use HHPack\Publisher\MessagePublisher;

final class EventPublisher
{

    private MessagePublisher<Message> $publisher;

    public function __construct(
        Traversable<Subscribable<Message>> $subscribers = []
    )
    {
        $this->publisher = new MessagePublisher($subscribers);
    }

    public async function migrationLoaded(ImmVector<Migration> $migrations): Awaitable<void>
    {
        await $this->publisher->publish(new MigrationLoadedEvent($migrations));
    }

    public async function migrationStart(string $query): Awaitable<void>
    {
        await $this->publisher->publish(new MigrationStartEvent($query));
    }

    public async function migrationSuccess(QueryResult $result): Awaitable<void>
    {
        await $this->publisher->publish(new MigrationSuccessEvent($result));
    }

}
