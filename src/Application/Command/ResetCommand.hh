<?hh //strict

/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Application\Command;

use HHPack\Getopt as cli;
use HHPack\Getopt\Parser\{OptionParser};
use HHPack\Migrate\Application\{Context, Command};
use HHPack\Migrate\{Migrator, SqlMigrationLoader, DatabaseClient};

final class ResetCommand extends MigrateSchemaCommand implements Command {

  public function __construct() {
    $this->usage = "migrate reset [OPTIONS]";
    $this->description = "Reset the schema to its initial state";
    $this->optionParser = cli\optparser(
      [
        cli\on(
          ['-h', '--help'],
          'Display help message',
          () ==> {
            $this->help = true;
          },
        ),
      ],
    );
  }

  public function run(Context $context): void {
    $this->optionParser->parse($context->cliArgs());

    if ($this->help) {
      $this->displayHelp();
    } else {
      \HH\Asio\join($this->resetToInitialState($context));
    }
  }

  private async function resetToInitialState(
    Context $context,
  ): Awaitable<void> {
    $migrator = $this->createMigrator($context);
    await $migrator->downgrade();
  }

}
