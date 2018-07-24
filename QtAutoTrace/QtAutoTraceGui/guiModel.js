var Steps = Object.freeze({
    HOME: 0,
    LOADING: 1,
    RESULTS: 2
});

var model = new function() {
    //#region Private

    var _error = null;
    var _step = Steps.HOME;
    var _inputFile = null;
//    var _forms = forms;

    //#endregion

    //#region Public
    this.error = function(error) {
        if (error &&  !this.hasError()) {
            this.step(Steps.RESULTS);
            _error = error.what();
            return;
        } else if (error && this.hasError()) {
            throw new Error("Need to clear the current error before setting a new one.");
        } else {
            return _error;
        }
    }

    this.hasError = function() {
        return this.error() !== null;
    }

    this.clearError = function() {
        _error = null;
    }

    this.step = function(step) {
        if (step) {
            if (this.hasError()) {
                // Can't navigate to any step while there is an error.
                throw new Error("Need to clear errors before navigating to a "
                    + "new step.");
            }

            if (!(step in Steps)) {
                throw new Error("Steps doesn't contain '" + step + "'.");
            } else {
                _step = step;
            }
        } else {
            return _step;
        }
    }
    //#endregion
}
