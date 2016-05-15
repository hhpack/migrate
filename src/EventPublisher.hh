<?hh //strict

namespace hhpack\migrate;

use hhpack\publisher\Message;
use hhpack\publisher\Subscribable;
use hhpack\publisher\MessagePublisher;

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

    public async function migrationStart(): Awaitable<void>
    {
        await $this->publisher->publish(new MigrationStartEvent());
    }

    public async function migrationSuccess(QueryResult $result): Awaitable<void>
    {
        await $this->publisher->publish(new MigrationSuccessEvent($result));
    }

}
