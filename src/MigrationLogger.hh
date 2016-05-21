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

use hhpack\color;
use hhpack\publisher\Message;
use hhpack\publisher\Subscribable;

final class MigrationLogger implements Subscribable<Message>
{

    public function loaded(MigrationLoadedEvent $event): void
    {
        color\info("Migration");
        color\info("migration found %d files",  $event->migrationCount());
    }

    public function start(MigrationStartEvent $event): void
    {
        color\info("query: %s",  $event->query());
    }

    public function success(MigrationSuccessEvent $event): void
    {
        color\info("success: %F seconds", $event->totalTime());
    }

}
