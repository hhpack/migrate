/**
 * This file is part of hhpack/migrate.
 *
 * (c) Noritaka Horio <holy.shared.design@gmail.com>
 *
 * This source file is subject to the MIT license that is bundled
 * with this source code in the file LICENSE.
 */

namespace HHPack\Migrate\Logger;

use HHPack\Color\{Color, ForegroundColor};
use HHPack\Migrate\{Logger, Output, Console};

final class ColoredLogger implements Logger {

  public function __construct(
    private Output $output = new Console(),
    private ColorFormatter $formatter = new ColorFormatter(),
  ) {}

  public function debug(string $message): void {
    $this->output->write($this->formatter->debug($message));
  }

  public function info(string $message): void {
    $this->output->write($this->formatter->info($message));
  }

  public function warn(string $message): void {
    $this->output->write($this->formatter->warn($message));
  }

  public function error(string $message): void {
    $this->output->writeError($this->formatter->error($message));
  }

}
