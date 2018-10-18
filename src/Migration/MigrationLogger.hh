<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Migration;

use HHPack\Migrate\{Logger};
use HHPack\Migrate\Event\{
  MigrationLoadedEvent,
  MigrationStartEvent,
  MigrationSuccessEvent,
};
use HHPack\Publisher\{Message, Subscribable};

final class MigrationLogger implements Subscribable<Message> {

  public function __construct(private Logger $logger) {}

  public function loaded(MigrationLoadedEvent $event): void {
    $this->logger->info("Database migration started\n");
    $this->logger->info(
      \sprintf("Migration found %d files\n\n", $event->migrationCount()),
    );
  }

  public function start(MigrationStartEvent $event): void {
    $this->logger->info(\sprintf("query: %s\n", $event->query()));
  }

  public function success(MigrationSuccessEvent $event): void {
    $this->logger
      ->info(\sprintf("success: %F seconds\n\n", $event->totalTime()));
  }

}
