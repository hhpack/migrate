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

use HHPack\Color as color;
use HHPack\Publisher\{ Message, Subscribable };

final class MigrationLogger implements Subscribable<Message>
{

    public function loaded(MigrationLoadedEvent $event): void
    {
        color\info("Database migration started");
        color\info("Migration found %d files\n",  $event->migrationCount());
    }

    public function start(MigrationStartEvent $event): void
    {
        color\info("query: %s\n",  $event->query());
    }

    public function success(MigrationSuccessEvent $event): void
    {
        color\info("success: %F seconds\n", $event->totalTime());
    }

}
